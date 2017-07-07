using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.WIP
{
    /// <summary>
    /// Disassembles the chosen dll, and generates source code for it.
    /// </summary>
    public class Disassemble : ICommand
    {
        public string GetHelp()
        {
            return "Disassembles the chosen file, and generates source code for it.";
        }

        public string GetName()
        {
            return "Disassemble";
        }

        public void Run(string Input)
        {
            
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] valid = { "disassemble", "dissassemble", "decode dll", "disassemble dll", "dissassemble dll" };

            return Util.Utility.DoesStringContain(Input, valid);
        }
    }
}
