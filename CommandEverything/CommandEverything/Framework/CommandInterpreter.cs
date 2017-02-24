using CommandEverything.Framework.Commands;
using CommandEverything.Framework.Util.Text;
using CommandEverything.Framework.WIP;
using System.Collections.Generic;
using System.Linq;

namespace CommandEverything.Framework
{
    /// <summary>
    /// Holds all available commands and deals with input/command execution.
    /// </summary>
    public static class CommandInterpreter
    {
        /// <summary>
        /// Holds all available commands.
        /// </summary>
        public static List<ICommand> AllCommands = new List<ICommand>();

        /// <summary>
        /// Adds a command to the all commands list.
        /// </summary>
        /// <param name="Command"></param>
        public static void AddCommand(ICommand Command)
        {
            if (Command != null)
            {
                AllCommands.Add(Command);
                AllCommands = AllCommands.OrderBy(o => o.GetName()).ToList();
            }
        }

        /// <summary>
        /// Attempts to run all commands with the specified input.
        /// </summary>
        /// <param name="Input"></param>
        public static void RecieveInput(string Input)
        {
            foreach (ICommand item in AllCommands)
            {
                if (item.ShouldRunThisCommand(Input.ToLower().Trim()))
                {
                    ConsoleWriter.WriteLine("-------------------- " + item.GetName() + " --------------------");
                    item.Run(Input.ToLower().Trim());
                    ConsoleWriter.WriteLine("-------------------- " + item.GetName() + " --------------------");
                    return;
                }
            }

            ConsoleWriter.WriteLine("Command not found");
        }

        /// <summary>
        /// Adds all standard commands to the console.
        /// </summary>
        public static void StartUp()
        {
            //Order matters. The higher on this list you are, the sooner you get to decide if you take the input and don't share it with others.
            AddCommand(new CMDCommands());
            AddCommand(new HelpCommand());
            AddCommand(new ExitCommand());
            AddCommand(new FreezeProgram());
            AddCommand(new NewWindow());
        }
    }
}