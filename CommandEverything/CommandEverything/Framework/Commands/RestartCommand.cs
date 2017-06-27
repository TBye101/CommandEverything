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
    /// Tells the system to restart itself.
    /// </summary>
    public class RestartCommand : ICommand
    {
        public string GetHelp()
        {
            return "Initiates system restart.";
        }

        public string GetName()
        {
            return "Restart";
        }

        public void Run(string Input)
        {
            ShutdownUtil a = new ShutdownUtil();
            a.ExitWindows(ShutdownUtil.EWX_RESTARTAPPS);
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = {"restart", "restart system", "restart computer", "restart windows", "reboot", "reboot system", "reboot computer", "reboot windows"};
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
