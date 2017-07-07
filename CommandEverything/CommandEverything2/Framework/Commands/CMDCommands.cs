using CommandEverything.Framework.CMD;
using CommandEverything.Framework.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Runs CMD commands if they are found.
    /// </summary>
    public class CMDCommands : ICommand
    {
        public string GetHelp()
        {
            return "Used to open a CMD command from this command prompt.";
        }

        public string GetName()
        {
            return "CMD";
        }

        public void Run(string Input)
        {
            Interaction.NewCMD();

        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "cmd" };

            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
