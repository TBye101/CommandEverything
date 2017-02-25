using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Tells the computer to shut down.
    /// </summary>
    public class ShutdownCommand : ICommand
    {
        public string GetHelp()
        {
            return "Initiates system shutdown.";
        }

        public string GetName()
        {
            return "Shutdown";
        }

        public void Run(string Input)
        {
            ManagementBaseObject mboShutdown = null;
            ManagementClass mcWin32 = new ManagementClass("Win32_OperatingSystem");
            mcWin32.Get();

            // You can't shutdown without security privileges
            mcWin32.Scope.Options.EnablePrivileges = true;
            ManagementBaseObject mboShutdownParams =
                     mcWin32.GetMethodParameters("Win32Shutdown");

            // Flag 1 means we want to shut down the system. Use "2" to reboot.
            mboShutdownParams["Flags"] = "1";
            mboShutdownParams["Reserved"] = "0";
            foreach (ManagementObject manObj in mcWin32.GetInstances())
            {
                mboShutdown = manObj.InvokeMethod("Win32Shutdown", mboShutdownParams, null);
            }
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "shutdown", "shutdown system", "power down", "power system down", "shutdown windows", "power down windows", "shut windows down" };
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
