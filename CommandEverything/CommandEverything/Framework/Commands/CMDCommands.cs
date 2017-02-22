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
            return "Used to run a CMD command from this command prompt. Just do 'CMD' followed by your arguments to CMD and it will receive your command";
        }

        public string GetName()
        {
            return "CMD";
        }

        public void Run(string Input)
        {
            Interaction.LaunchCmdCommand(Utility.RemoveWordFromString(Input, "CMD"));

        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "cmd" };

            return Utility.DoesStringContain(Input, Valid);
        }
    }
}
