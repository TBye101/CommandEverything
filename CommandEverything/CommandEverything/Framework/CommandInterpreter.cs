using CommandEverything.Framework.Commands;
using System;
using System.Collections.Generic;
using System.Linq;

namespace CommandEverything.Framework
{
    /// <summary>
    /// Holds all availible commands and deals with input/command execution.
    /// </summary>
    public static class CommandInterpreter
    {
        /// <summary>
        /// Holds all availible commands.
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
            bool Found = false;

            foreach (ICommand item in AllCommands)
            {
                if (item.ShouldRunThisCommand(Input.ToLower().Trim()))
                {
                    Found = true;

                    Console.WriteLine("-------------------- " + item.GetName() + " --------------------");
                    item.Run(Input.ToLower().Trim());
                    Console.WriteLine("-------------------- " + item.GetName() + " --------------------");
                }
            }

            if (!Found)
            {
                Console.WriteLine("Command not found");
            }
        }

        /// <summary>
        /// Adds all standard commands to the console.
        /// </summary>
        public static void StartUp()
        {
            AddCommand(new ExitCommand());
            AddCommand(new HelpCommand());
            AddCommand(new TimeCommand());
        }
    }
}