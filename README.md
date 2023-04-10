simple systemcall tracer

Usage:

    nrtrace [Option] [CMD [ARGs...]]

Option:

    -p  : Trace target thread ids, separated comma. Don't trace other thread in same process.
    -e  : Print syscall names, separated comma. Default is all print.
    --ee: Print syscall inclusive names, separated comma. Default is all print.
    -E  : No print syscall names, separated comma.
    --EE: No print syscall inclusive names, separated comma.
    -s  : Simple print syscalll names, separated comma.
    --ss: Simple print syscall inclusive names, separated comma.


libc以外のライブラリには依存していない。
libcはstatic linkするので、最終生成物1ファイルのみで動作する。
Android(bionic libc)など、glic以外にも対応しているはず。
ただし、ptraceの都合上、Linux 5.3以降必須。

PTRACE_GET_SYSCALL_INFO を PTRACE_GETREGS に変更すれば Linux 3.4以降に緩和。
この場合、syscallのin/outがわからなくなるので、syscallの番号の変化で検知する。
戻り値がエラーかどうかは、-1 ～ -4095の範囲内か否かで判定する。
アーキテクチャによっては第1引数と戻り値を同じレジスタに格納するため、syscallのout時に参照するように引数はin時に保存しておく。
32bit/64bitの判定は PTRACE_GETREGS で読みだしたバッファサイズで判定する。
レジスタの構造体は arch/x86/include/uapi/asm/ptrace.h など参照。

更に PTRACE_SEIZE を PTRACE_ATTACH に変更すれば Linux 2.6以降に緩和。
ただし、traceeの停止要因(ptrace-event、group-stop、syscall-stop、signal)の切り分け処理が増える、詳細はptrace(2)参照。
