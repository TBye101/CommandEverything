using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// Used to shutdown or restart the computer.
    /// </summary>
    public class ShutdownUtil
    {
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        internal struct TokPriv1Luid
        {
            public int Count;
            public long Luid;
            public int Attr;
        }

        [DllImport("kernel32.dll", ExactSpelling = true)]
        internal static extern IntPtr GetCurrentProcess();

        [DllImport("advapi32.dll", ExactSpelling = true, SetLastError = true)]
        internal static extern bool OpenProcessToken(IntPtr h, int acc, ref IntPtr
        phtok);

        [DllImport("advapi32.dll", SetLastError = true)]
        internal static extern bool LookupPrivilegeValue(string host, string name,
        ref long pluid);

        [DllImport("advapi32.dll", ExactSpelling = true, SetLastError = true)]
        internal static extern bool AdjustTokenPrivileges(IntPtr htok, bool disall,
        ref TokPriv1Luid newst, int len, IntPtr prev, IntPtr relen);

        [DllImport("user32.dll", ExactSpelling = true, SetLastError = true)]
        internal static extern bool ExitWindowsEx(int flg, int rea);

        internal const int SE_PRIVILEGE_ENABLED = 0x00000002;
        internal const int TOKEN_QUERY = 0x00000008;
        internal const int TOKEN_ADJUST_PRIVILEGES = 0x00000020;
        internal const string SE_SHUTDOWN_NAME = "SeShutdownPrivilege";

        /// <summary>
        /// Flag tells the computer to log off the current user.
        /// </summary>
        internal const int EWX_LOGOFF = 0x00000000;

        /// <summary>
        /// Flag tells the computer to shutdown.
        /// </summary>
        internal const int EWX_SHUTDOWN = 0x00000001;

        /// <summary>
        /// Flag tells the computer to reboot.
        /// </summary>
        internal const int EWX_REBOOT = 0x00000002;

        /// <summary>
        /// Has no effect if terminal services is enabled. Makes the system to not send the WM_QUERYENDSESSION message. This can cause applications to lose data. Causes the user to logout quickily.
        /// </summary>
        internal const int EWX_FORCE = 0x00000004;

        internal const int EWX_POWEROFF = 0x00000008;

        /// <summary>
        /// Forces processes to terminate if they do not respond to system messages.
        /// </summary>
        internal const int EWX_FORCEIFHUNG = 0x00000010;

        /// <summary>
        /// Partially shuts down the computer for faster startup times.
        /// </summary>
        internal const int EWX_HYBRID_SHUTDOWN = 0x00400000;

        /// <summary>
        /// Restarts the computer, and launches applications register applications.
        /// </summary>
        internal const int EWX_RESTARTAPPS = 0x00000040;

        public void ExitWindows(int flg)
        {
            bool ok;
            TokPriv1Luid tp;
            IntPtr hproc = GetCurrentProcess();
            IntPtr htok = IntPtr.Zero;
            ok = OpenProcessToken(hproc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, ref htok);
            tp.Count = 1;
            tp.Luid = 0;
            tp.Attr = SE_PRIVILEGE_ENABLED;
            ok = LookupPrivilegeValue(null, SE_SHUTDOWN_NAME, ref tp.Luid);
            ok = AdjustTokenPrivileges(htok, false, ref tp, 0, IntPtr.Zero, IntPtr.Zero);
            ok = ExitWindowsEx(flg, 0);
        }
    }
}
