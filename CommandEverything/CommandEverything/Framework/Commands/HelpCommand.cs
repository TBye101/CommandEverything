using CommandEverything.Framework.Util;
using System;

namespace CommandEverything.Framework.Commands
{
    /// <summary>
    /// Gets the names of all commands, and can give help information about them.
    /// </summary>
    public class HelpCommand : ICommand
    {
        /// <summary>
        /// Returns help information about this command.
        /// </summary>
        /// <returns></returns>
        public string GetHelp()
        {
            return "This command is used to get help information about other commands.";
        }

        /// <summary>
        /// Returns the name of this command.
        /// </summary>
        /// <returns></returns>
        public string GetName()
        {
            return "Help";
        }

        /// <summary>
        /// Runs this command.
        /// </summary>
        /// <param name="Input"></param>
        public void Run(string Input)
        {
            if (Input == "help")
            {
                this.SpitOutCommandNames();
            }
            else
            {
                string Processed = Utility.RemoveWordFromString(Input, "help");
                this.GetSpecificCommandHelp(Processed);
            }
        }

        /// <summary>
        /// Writes help about a specific command to the console.
        /// </summary>
        /// <param name="ProcessedInput"></param>
        private void GetSpecificCommandHelp(string ProcessedInput)
        {
            bool Found = false;

            foreach (ICommand item in CommandInterpreter.AllCommands)
            {
                if (item.GetName() == ProcessedInput.Trim())
                {
                    Console.WriteLine(item.GetHelp());
                    Found = true;
                }
            }

            if (!Found)
            {
                Console.WriteLine("Command not found");
            }
        }

        /// <summary>
        /// Writes to the console the names of all availible commands.
        /// </summary>
        private void SpitOutCommandNames()
        {
            foreach (ICommand item in CommandInterpreter.AllCommands)
            {
                Console.WriteLine(item.GetName());
            }
        }

        /// <summary>
        /// Determins if this command should run.
        /// </summary>
        /// <param name="Input"></param>
        /// <returns></returns>
        public bool ShouldRunThisCommand(string Input)
        {
            if (Input.Contains("help"))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}