/************************************************************
This file is generated automatically by an awk script - DO NOT EDIT
************************************************************/


typedef struct {
	int	number;
	char *	name;
} SYSCALL_NAME;

SYSCALL_NAME SyscallNameTab[] = {
	-26, "CONDOR_put_file_stream",
	-25, "CONDOR_get_file_stream",
	-24, "CONDOR_get_file_chunk",
	-23, "CONDOR_suspended",
	-22, "CONDOR_pvm_task_info",
	-21, "CONDOR_new_connection",
	-20, "CONDOR_pvm_info",
	-19, "CONDOR_subproc_status",
	-18, "CONDOR_work_request",
	-17, "CONDOR_send_core",
	-16, "CONDOR_perm_error",
	-15, "CONDOR_send_rusage",
	-14, "CONDOR_send_file",
	-13, "CONDOR_get_file",
	-12, "CONDOR_extern_name",
	-11, "CONDOR_free_fs_blocks",
	-10, "CONDOR_image_size",
	-9, "CONDOR_processlogging",
	-8, "CONDOR_reallyexit",
	-7, "CONDOR_updaterusage",
	-6, "CONDOR_getwd",
	-4, "CONDOR_getegid",
	-3, "CONDOR_geteuid",
	-1, "CONDOR_wait3",
	0, "CONDOR_accept",
	1, "CONDOR_access",
	2, "CONDOR_acct",
	3, "CONDOR_adjtime",
	4, "CONDOR_async_daemon",
	5, "CONDOR_bind",
	6, "CONDOR_chdir",
	7, "CONDOR_chmod",
	8, "CONDOR_chown",
	9, "CONDOR_chroot",
	10, "CONDOR_close",
	11, "CONDOR_connect",
	12, "CONDOR_creat",
	13, "CONDOR_dup",
	14, "CONDOR_dup2",
	15, "CONDOR_execv",
	16, "CONDOR_execve",
	17, "CONDOR__exit",
	18, "CONDOR_exportfs",
	19, "CONDOR_fchmod",
	20, "CONDOR_fchown",
	21, "CONDOR_fcntl",
	22, "CONDOR_flock",
	23, "CONDOR_fork",
	24, "CONDOR_fstat",
	25, "CONDOR_fstatfs",
	26, "CONDOR_fsync",
	27, "CONDOR_ftruncate",
	28, "CONDOR_getdirentries",
	29, "CONDOR_getdomainname",
	30, "CONDOR_getdopt",
	31, "CONDOR_getdtablesize",
	32, "CONDOR_getfh",
	33, "CONDOR_getgid",
	34, "CONDOR_getgroups",
	35, "CONDOR_gethostid",
	36, "CONDOR_gethostname",
	37, "CONDOR_getitimer",
	38, "CONDOR_getpagesize",
	39, "CONDOR_getpeername",
	40, "CONDOR_getpgrp",
	41, "CONDOR_getpid",
	42, "CONDOR_getpriority",
	43, "CONDOR_getrlimit",
	44, "CONDOR_getrusage",
	45, "CONDOR_getsockname",
	46, "CONDOR_getsockopt",
	47, "CONDOR_gettimeofday",
	48, "CONDOR_getuid",
	49, "CONDOR_ioctl",
	50, "CONDOR_kill",
	51, "CONDOR_killpg",
	52, "CONDOR_link",
	53, "CONDOR_listen",
	54, "CONDOR_lseek",
	55, "CONDOR_lstat",
	56, "CONDOR_madvise",
	57, "CONDOR_mincore",
	58, "CONDOR_mkdir",
	59, "CONDOR_mknod",
	60, "CONDOR_mmap",
	61, "CONDOR_mount",
	62, "CONDOR_mprotect",
	63, "CONDOR_mremap",
	64, "CONDOR_munmap",
	65, "CONDOR_nfssvc",
	66, "CONDOR_open",
	67, "CONDOR_pipe",
	68, "CONDOR_profil",
	69, "CONDOR_ptrace",
	70, "CONDOR_quotactl",
	71, "CONDOR_read",
	72, "CONDOR_readlink",
	73, "CONDOR_readv",
	74, "CONDOR_reboot",
	75, "CONDOR_recv",
	76, "CONDOR_recvfrom",
	77, "CONDOR_recvmsg",
	78, "CONDOR_rename",
	79, "CONDOR_rmdir",
	80, "CONDOR_sbrk",
	81, "CONDOR_select",
	82, "CONDOR_send",
	83, "CONDOR_sendmsg",
	84, "CONDOR_sendto",
	85, "CONDOR_setdomainname",
	86, "CONDOR_setdopt",
	87, "CONDOR_setgroups",
	88, "CONDOR_sethostid",
	89, "CONDOR_sethostname",
	90, "CONDOR_setitimer",
	91, "CONDOR_setpgrp",
	92, "CONDOR_setpriority",
	93, "CONDOR_setregid",
	94, "CONDOR_setreuid",
	95, "CONDOR_setrlimit",
	96, "CONDOR_setsockopt",
	97, "CONDOR_settimeofday",
	98, "CONDOR_shutdown",
	99, "CONDOR_sigblock",
	100, "CONDOR_sigpause",
	101, "CONDOR_sigreturn",
	102, "CONDOR_sigsetmask",
	103, "CONDOR_sigstack",
	104, "CONDOR_sigvec",
	105, "CONDOR_socket",
	106, "CONDOR_socketpair",
	107, "CONDOR_sstk",
	108, "CONDOR_stat",
	109, "CONDOR_statfs",
	110, "CONDOR_swapon",
	111, "CONDOR_symlink",
	112, "CONDOR_sync",
	113, "CONDOR_truncate",
	114, "CONDOR_umask",
	115, "CONDOR_unlink",
	116, "CONDOR_umount",
	117, "CONDOR_utimes",
	118, "CONDOR_vhangup",
	119, "CONDOR_wait",
	120, "CONDOR_write",
	121, "CONDOR_writev",
	122, "CONDOR_accessx",
	123, "CONDOR_audit",
	124, "CONDOR_auditbin",
	125, "CONDOR_auditevents",
	126, "CONDOR_auditlog",
	127, "CONDOR_auditobj",
	128, "CONDOR_auditproc",
	129, "CONDOR_brk",
	130, "CONDOR_chacl",
	131, "CONDOR_chownx",
	132, "CONDOR_chpriv",
	133, "CONDOR_disclaim",
	134, "CONDOR_faccessx",
	135, "CONDOR_fchacl",
	136, "CONDOR_fchownx",
	137, "CONDOR_fchpriv",
	138, "CONDOR_fclear",
	139, "CONDOR_frevoke",
	140, "CONDOR_fscntl",
	141, "CONDOR_fstatacl",
	142, "CONDOR_fstatpriv",
	143, "CONDOR_fstatx",
	144, "CONDOR_getargs",
	145, "CONDOR_getdirent",
	146, "CONDOR_getevars",
	147, "CONDOR_getgidx",
	148, "CONDOR_kgetpgrp",
	149, "CONDOR_getpri",
	150, "CONDOR_getpriv",
	151, "CONDOR_getproc",
	152, "CONDOR_getuidx",
	153, "CONDOR_getuser",
	154, "CONDOR_kfcntl",
	155, "CONDOR_kioctl",
	156, "CONDOR_knlist",
	157, "CONDOR_kreadv",
	158, "CONDOR_kwaitpid",
	159, "CONDOR_kwritev",
	160, "CONDOR_load",
	161, "CONDOR_loadbind",
	162, "CONDOR_loadquery",
	163, "CONDOR_mntctl",
	164, "CONDOR_openx",
	165, "CONDOR_pause",
	166, "CONDOR_plock",
	167, "CONDOR_poll",
	168, "CONDOR_privcheck",
	169, "CONDOR_psdanger",
	170, "CONDOR_revoke",
	171, "CONDOR_absinterval",
	172, "CONDOR_getinterval",
	173, "CONDOR_gettimer",
	174, "CONDOR_gettimerid",
	175, "CONDOR_incinterval",
	176, "CONDOR_reltimerid",
	177, "CONDOR_resabs",
	178, "CONDOR_resinc",
	179, "CONDOR_restimer",
	180, "CONDOR_settimer",
	181, "CONDOR_nsleep",
	182, "CONDOR_seteuid",
	183, "CONDOR_setgid",
	184, "CONDOR_setgidx",
	185, "CONDOR_setpgid",
	186, "CONDOR_setpri",
	187, "CONDOR_setpriv",
	188, "CONDOR_setsid",
	189, "CONDOR_setuid",
	190, "CONDOR_setuidx",
	191, "CONDOR_shmctl",
	192, "CONDOR_shmget",
	193, "CONDOR_shmat",
	194, "CONDOR_shmdt",
	195, "CONDOR_msgctl",
	196, "CONDOR_msgget",
	197, "CONDOR_msgsnd",
	198, "CONDOR_msgrcv",
	199, "CONDOR_msgxrcv",
	200, "CONDOR_semctl",
	201, "CONDOR_semget",
	202, "CONDOR_semop",
	203, "CONDOR_sigaction",
	204, "CONDOR_sigcleanup",
	205, "CONDOR_sigprocmask",
	206, "CONDOR_sigsuspend",
	207, "CONDOR_sigpending",
	208, "CONDOR_statacl",
	209, "CONDOR_statpriv",
	210, "CONDOR_statx",
	211, "CONDOR_swapoff",
	212, "CONDOR_swapqry",
	213, "CONDOR_sysconfig",
	214, "CONDOR_times",
	215, "CONDOR_ulimit",
	216, "CONDOR_uname",
	217, "CONDOR_unameu",
	218, "CONDOR_unamex",
	219, "CONDOR_unload",
	220, "CONDOR_usrinfo",
	221, "CONDOR_ustat",
	222, "CONDOR_uvmount",
	223, "CONDOR_vmount",
	224, "CONDOR_Trconflag",
	225, "CONDOR_trchook",
	226, "CONDOR_trchk",
	227, "CONDOR_trchkt",
	228, "CONDOR_trchkl",
	229, "CONDOR_trchklt",
	230, "CONDOR_trchkg",
	231, "CONDOR_trchkgt",
	232, "CONDOR_trcgen",
	233, "CONDOR_trcgent",
	234, "CONDOR_getppid",
	235, "CONDOR_lockf",
	236, "CONDOR_unmount",
	237, "CONDOR_sigvector",
	238, "CONDOR_getpgrp2",
	239, "CONDOR_utssys",
	240, "CONDOR_setresuid",
	241, "CONDOR_setresgid",
	242, "CONDOR_utime",
	243, "CONDOR_wait4",
	244, "CONDOR_exec_with_loader",
	245, "CONDOR_getlogin",
	246, "CONDOR_setlogin",
	247, "CONDOR_fchdir",
	248, "CONDOR_getfsstat",
	249, "CONDOR_vfork",
	250, "CONDOR_table",
	251, "CONDOR_lchown",
	252, "CONDOR_mvalid",
	253, "CONDOR_getaddressconf",
	254, "CONDOR_msync",
	255, "CONDOR_getpgid",
	256, "CONDOR_uswitch",
	257, "CONDOR_getsysinfo",
	258, "CONDOR_setsysinfo",
	259, "CONDOR_getdents",
};

char *
_condor_syscall_name( n )
int n;
{
	static char error[512];
	int		i;
	int		lim = sizeof( SyscallNameTab) / sizeof( SyscallNameTab[0] );

	for( i=0; i<lim; i++ ) {
		if( SyscallNameTab[i].number == n ) {
			return SyscallNameTab[i].name;
		}
	}
	sprintf( error, "Unknown System Call (%d)", n );
	return error;
}
