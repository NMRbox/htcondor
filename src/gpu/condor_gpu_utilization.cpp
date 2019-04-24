#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <vector>

// This is a mess, but since Windows.h isn't as safe to include as
// unistd.h and dlfcn.h, we do the song-and-dance routine here, so
// that sources files including "condor_common.h", which has a
// different song-and-dance, can safely use the pi_* headers.  *sigh*

#if       defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOSERVICE
#define NOMCX
#define NOIME
#include <Windows.h>
#endif /* defined(WIN32) */

#include "pi_sleep.h"
#include "pi_dynlink.h"

#include "nvml_stub.h"
#include "cuda_header_doc.h"
#include "cuda_device_enumeration.h"

unsigned debug = 0;
time_t reportInterval = 10;

int compareSamples( const void * vpA, const void * vpB ) {
	const nvmlSample_t * a = (const nvmlSample_t *)vpA;
	const nvmlSample_t * b = (const nvmlSample_t *)vpB;
	if( a->timeStamp < b->timeStamp ) {
		return -1;
	} else if( a->timeStamp == b->timeStamp ) {
		return 0;
	} else {
		return 1;
	}
}

#ifndef WINDOWS
void fail() __attribute__((__noreturn__));
#endif

void fail() {
	fprintf( stderr, "Hanging to prevent process churn.\n" );
	while( 1 ) { sleep( 1024 ); }
}

nvmlReturn_t getElapsedTimeForDevice( nvmlDevice_t d, unsigned long long * lastSample, unsigned long long * elapsedTime, unsigned maxSampleCount, unsigned long long * runningSampleCount ) {
	if( elapsedTime == NULL || lastSample == NULL ) {
		return NVML_ERROR_INVALID_ARGUMENT;
	}

	unsigned int sampleCount = maxSampleCount;
	std::vector<nvmlSample_t> samples;
	samples.resize(maxSampleCount);
	nvmlValueType_t sampleValueType;

	nvmlReturn_t r = nvmlDeviceGetSamples( d, NVML_GPU_UTILIZATION_SAMPLES, * lastSample, & sampleValueType, & sampleCount, &samples[0] );
	switch( r ) {
		case NVML_ERROR_NOT_FOUND:
			return NVML_SUCCESS;
		case NVML_SUCCESS:
			break;
		default:
			return r;
	}
	(* runningSampleCount) += sampleCount;

	// Samples are usually but not always in order.
	qsort( &samples[0], sampleCount, sizeof( nvmlSample_t ), compareSamples );

	// Convert the percentage to elapsed time, since that's what we
	// actually care about (and the data representation is simpler).
	if( (*lastSample) == 0 && sampleCount > 0 ) {
		(*lastSample) = samples[0].timeStamp;
	}
	for( unsigned i = 0; i < sampleCount; ++i ) {
		/* debug fprintf( stderr, "%llu + (%llu - %llu) * %f = ",
			* elapsedTime, samples[i].timeStamp, * lastSample,
			(double)(samples[i].sampleValue.uiVal)/100.0 ); */

		(*elapsedTime) += (unsigned long long)(
			(samples[i].timeStamp - (*lastSample)) *
			((double)(samples[i].sampleValue.uiVal)/100.0)
		);

		/* debug fprintf( stderr, "%llu\n", * elapsedTime ); */

		(*lastSample) = samples[i].timeStamp;
	}

	return NVML_SUCCESS;
}

int main() {
	// The actual filtering is done by the startd on the basis
	// of the SlotMergeConstraint we set for each ad we emit.
#if defined(WINDOWS)
	_putenv( "CUDA_VISIBLE_DEVICES=" );
#else
	unsetenv( "CUDA_VISIBLE_DEVICES" );
#endif

	auto cuda_handle = setCUDAFunctionPointers();
	if(! cuda_handle) { fail(); }
	if( cuInit(0) != CUDA_SUCCESS ) {
		fprintf( stderr, "cuInit(0) failed, aborting.\n" );
		fail();
	}

	auto nvml_handle = setNVMLFunctionPointers();
	if(! nvml_handle) { fail();	}
	if( nvmlInit() != NVML_SUCCESS ) {
		fprintf( stderr, "nvmlInit() failed, aborting.\n" );
		fail();
	}

	std::vector< BasicProps > cudaDevices;
	if(! enumerateCUDADevices( cudaDevices )) {
		fprintf( stderr, "Failed to enumerate CUDA devices, aborting.\n" );
		fail();
	}

	unsigned int deviceCount = 0;
	if( nvmlDeviceGetCount(& deviceCount) != NVML_SUCCESS ) {
		fprintf( stderr, "nvmlDeviceGetCount() failed, aborting.\n" );
		fail();
	}
	if( deviceCount != cudaDevices.size() ) {
		fprintf( stderr, "nvmlDeviceGetCount() disagrees with CUDA device count, aborting.\n" );
		fail();
	}
	if( deviceCount <= 0 ) {
		fprintf( stderr, "Found 0 or fewer devices, aborting.\n" );
		fail();
	}

	nvmlReturn_t r;
	std::vector<nvmlDevice_t> devices; devices.resize(deviceCount);
	std::vector<unsigned int> maxSampleCounts; maxSampleCounts.resize(deviceCount);
	std::vector<unsigned long long> memoryUsage; memoryUsage.resize(deviceCount);
	std::vector<unsigned long long> lastSamples; lastSamples.resize(deviceCount);
	std::vector<unsigned long long> firstSamples; firstSamples.resize(deviceCount);
	std::vector<unsigned long long> elapsedTimes; elapsedTimes.resize(deviceCount);
	std::vector<unsigned long long> runningSampleCounts; runningSampleCounts.resize(deviceCount);
	for( unsigned i = 0; i < deviceCount; ++i ) {
		r = nvmlDeviceGetHandleByPciBusId( cudaDevices[i].pciId, &(devices[i]) );
		if( r == NVML_ERROR_NO_PERMISSION ) {
			// Ignore devices we don't have permission for rather than fail.
			continue;
		} else if( r != NVML_SUCCESS ) {
			fprintf( stderr, "nvmlGetDeviceHandleByIndex(%u) failed (%d: %s), aborting.\n", i, r, nvmlErrorString( r ) );
			fail();
		}

		lastSamples[i] = 0;
		firstSamples[i] = 0;
		elapsedTimes[i] = 0;
		runningSampleCounts[i] = 0;

		nvmlValueType_t sampleValueType;
		r = nvmlDeviceGetSamples( devices[i], NVML_GPU_UTILIZATION_SAMPLES, 0, & sampleValueType, & maxSampleCounts[i], NULL );
		if( r != NVML_SUCCESS ) {
			fprintf( stderr, "nvmlDeviceGetSamples(%u) failed while querying for the max sample count (%d: %s), aborting.\n", i, r, nvmlErrorString( r ) );
			fail();
		}
		if( sampleValueType != NVML_VALUE_TYPE_UNSIGNED_INT ) {
			fprintf( stderr, "nvmlDeviceGetSamples(%u) returned an unexpected type (%d) of sample when querying for the max sample count, aborting.\n", i, sampleValueType );
			fail();
		}

		memoryUsage[i] = 0;
	}

	// We deliberately ignore the first set of samples.  Partly, I think we
	// can claim that they're from before we started monitoring.  More
	// importantly, at least on a Tesla K40c (driver 384.81), initializing
	// the NVML library causes a one-second 99% usage spike on an other-
	// wise idle GPU.  So we'll ignore as much of that as we easily can.
	for( unsigned i = 0; i < deviceCount; ++i ) {
		getElapsedTimeForDevice( devices[i], &lastSamples[i], &elapsedTimes[i], maxSampleCounts[i], &runningSampleCounts[i] );
		firstSamples[i] = lastSamples[i];
		elapsedTimes[i] = 0;
		runningSampleCounts[i] = 0;
	}


	// The documented minimum duration for a sample is 1/6 of a second,
	// so the minimum sampling rate in seconds is the size of the smallest
	// sample buffer divided by six (assuming that each max sample count
	// is minimal).
	unsigned minMaxSampleCount = (unsigned)-1;
	for( unsigned i = 0; i < deviceCount; ++i ) {
		if( maxSampleCounts[i] < minMaxSampleCount ) {
			minMaxSampleCount = maxSampleCounts[i];
		}
	}
	unsigned long long sampleIntervalMicrosec = (minMaxSampleCount * 1000000)/6;


	time_t lastReport = time( NULL );
	while( 1 ) {
		// Take samples three times as often as we have to minimize aliasing.
		usleep(sampleIntervalMicrosec / 3 );

		for( unsigned i = 0; i < deviceCount; ++i ) {
			r = getElapsedTimeForDevice( devices[i], &lastSamples[i], &elapsedTimes[i], maxSampleCounts[i], &runningSampleCounts[i] );
			if( r != NVML_SUCCESS ) {
				fprintf( stderr, "getElapsedTimeForDevice(%u) failed (%d: %s), aborting.\n", i, r, nvmlErrorString( r ) );
				fail();
			}

			if( debug ) {
				fprintf( stdout, "device %u: %llu / %llu = %u%%.\n",
					i, elapsedTimes[i], lastSamples[i] - firstSamples[i],
					(unsigned)(((double)elapsedTimes[i]) / (lastSamples[i] - firstSamples[i]) * 100)
				);
			}

			nvmlMemory_t mi = { 0, 0, 0 };
			r = nvmlDeviceGetMemoryInfo( devices[i], &mi );
			if( r != NVML_SUCCESS ) {
				fprintf( stderr, "getDeviceGetMemoryInfo(%u) failed (%d: %s), aborting.\n", i, r, nvmlErrorString( r ) );
				fail();
			}
			if( mi.used > memoryUsage[i] ) {
				memoryUsage[i] = mi.used;
			}
		}

		if( time( NULL ) - lastReport >= reportInterval ) {
			for( unsigned i = 0; i < deviceCount; ++i ) {
				fprintf( stdout, "SlotMergeConstraint = StringListMember( \"CUDA%u\", AssignedGPUs )\n", i );
				fprintf( stdout, "UptimeGPUsSeconds = %.6f\n", elapsedTimes[i] / 1000000.0 );
				fprintf( stdout, "UptimeGPUsMemoryPeakUsage = %llu\n", (memoryUsage[i] + (1024 * 1024) -1) / (1024 * 1024) );
				fprintf( stdout, "- GPUsSlot%u\n", i );
				fflush( stdout );

				/* debug fprintf( stderr, "Used %lu samples to cover %lu microseconds (%2.2f samples per second)\n",
					runningSampleCounts[i], lastSamples[i] - firstSamples[i],
					1000000/((lastSamples[i] - firstSamples[i])/(1.0 * runningSampleCounts[i])) ); */

				// Report only the usage for each reporting period.
				elapsedTimes[i] = 0;
				firstSamples[i] = lastSamples[i];
				memoryUsage[i] = 0;
				runningSampleCounts[i] = 0;
			}
			lastReport = time( NULL );
		}
	}

	r = nvmlShutdown();
	if( r != NVML_SUCCESS ) {
		fprintf( stderr, "nvmlShutdown() failed (%d: %s), aborting.\n", r, nvmlErrorString( r ) );
		return 1;
	}

	dlclose( nvml_handle );
	dlclose( cuda_handle );
	return 0;
}
