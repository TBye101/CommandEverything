using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Freezes the specified running program.
    /// </summary>
    public class FreezeProgram : ICommand
    {
        public string GetHelp()
        {
            return "Pauses the execution of the specified program.";
        }

        public string GetName()
        {
            return "Freeze Program";
        }

        public void Run(string Input)
        {
            if (Utility.DoesStringContain(Input, new string[] {"all", "everything", "all processes", "all programs", }))
            {
                foreach (Process item in Process.GetProcesses())
                {
                    this.SuspendProcess(item.Id);
                }
            }
            else
            {
                //Get Program ID here, then freeze it
            }
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "freeze", "suspend", "freeze program", "suspend program", "stop program" };
            return Utility.DoesStringContain(Input, Valid);
        }

        [Flags]
        public enum ThreadAccess : int
        {
            TERMINATE = (0x0001),
            SUSPEND_RESUME = (0x0002),
            GET_CONTEXT = (0x0008),
            SET_CONTEXT = (0x0010),
            SET_INFORMATION = (0x0020),
            QUERY_INFORMATION = (0x0040),
            SET_THREAD_TOKEN = (0x0080),
            IMPERSONATE = (0x0100),
            DIRECT_IMPERSONATION = (0x0200)
        }

        [DllImport("kernel32.dll")]
        static extern IntPtr OpenThread(ThreadAccess dwDesiredAccess, bool bInheritHandle, uint dwThreadId);
        [DllImport("kernel32.dll")]
        static extern uint SuspendThread(IntPtr hThread);
        [DllImport("kernel32.dll")]
        static extern int ResumeThread(IntPtr hThread);
        [DllImport("kernel32", CharSet = CharSet.Auto, SetLastError = true)]
        static extern bool CloseHandle(IntPtr handle);


        private void SuspendProcess(int pid)
        {
            Process process = Process.GetProcessById(pid);

            if (process.ProcessName == string.Empty)
            {
                return;
            }

            foreach (ProcessThread pT in process.Threads)
            {
                IntPtr pOpenThread = OpenThread(ThreadAccess.SUSPEND_RESUME, false, (uint)pT.Id);

                if (pOpenThread == IntPtr.Zero)
                {
                    continue;
                }

                SuspendThread(pOpenThread);

                CloseHandle(pOpenThread);
            }
        }

        public void ResumeProcess(int pid)
        {
            Process process = Process.GetProcessById(pid);

            if (process.ProcessName == string.Empty)
            {
                return;
            }

            foreach (ProcessThread pT in process.Threads)
            {
                IntPtr pOpenThread = OpenThread(ThreadAccess.SUSPEND_RESUME, false, (uint)pT.Id);

                if (pOpenThread == IntPtr.Zero)
                {
                    continue;
                }

                int suspendCount = 0;

                do
                {
                    suspendCount = ResumeThread(pOpenThread);
                } while (suspendCount > 0);

                CloseHandle(pOpenThread);
            }
        }
    }
}
