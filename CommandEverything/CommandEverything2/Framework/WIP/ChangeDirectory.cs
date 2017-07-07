using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Changes the directory this program is operating out of.
    /// </summary>
    public class ChangeDirectory : ICommand
    {
        public string GetHelp()
        {
            return "Changes the directory that this program is operating out of.";
        }

        public string GetName()
        {
            return "CD";
        }

        public void Run(string Input)
        {
            throw new NotImplementedException();
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "cd", "change directory", "change folder" };
            return Utility.DoesStringContain(Input, Valid);
        }
    }
}