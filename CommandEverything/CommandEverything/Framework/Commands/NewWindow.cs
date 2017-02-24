using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.WIP
{
    /// <summary>
    /// Opens a new command window.
    /// </summary>
    public class NewWindow : ICommand
    {
        public string GetHelp()
        {
            return "Opens a new prompt.";
        }

        public string GetName()
        {
            return "New";
        }

        public void Run(string Input)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo("CommandEverything.exe")
            {
                WindowStyle = ProcessWindowStyle.Normal,
                UseShellExecute = true,
                RedirectStandardOutput = false,
                CreateNoWindow = true
            };

            Process a = Process.Start(startInfo);
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "new", "new prompt" };
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
