using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

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
            ShutdownUtil a = new ShutdownUtil();
            a.ExitWindows(ShutdownUtil.EWX_SHUTDOWN);
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "shutdown", "shutdown system", "power down", "power system down", "shutdown windows", "power down windows", "shut windows down" };
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
