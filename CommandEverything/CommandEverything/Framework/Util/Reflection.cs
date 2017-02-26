using CommandEverything.Framework.Util.Text;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// Holds all sorts of reflection utilities.
    /// </summary>
    public class Reflection
    {
        StreamWriter Stream;
        Assembly Asm;

        /// <summary>
        /// Generates data about the specified assembly, and saves it to the specified file.
        /// Also logs the data generated to the console.
        /// </summary>
        /// <param name="File"></param>
        /// <param name="Asm"></param>
        public void GenerateData(StreamWriter File, Assembly Asmm)
        {
            this.Stream = File;
            Asm = Asmm;

            this.Log("-Assembly Location: " + Asmm.CodeBase);
            this.CustomAttributes();
            this.DeclaredTypes();
            this.ReferencedAssemblies();
            this.Log("-Entry Point: " + Asm.EntryPoint.Name);
        }

        private void ReferencedAssemblies()
        {
            this.Log("--Referenced Assemblies:");

            foreach (AssemblyName item in Asm.GetReferencedAssemblies())
            {
                this.Log(item.FullName);
                this.Log("---" + item.ProcessorArchitecture.ToString()));
                this.Log("---" + item.Version.ToString());
                this.Log("---" + item.CodeBase.ToString());
            }
        }

        private void TypeInfo(TypeInfo Typ)
        {
            this.Log(Typ.FullName);
            this.Log("--Base type: " + Typ.BaseType);
            this.Log("--Type: " + Typ.GetType());
            this.Log("--Type size: " + System.Runtime.InteropServices.Marshal.SizeOf(Typ));
            this.Log("**Nested Types:**");

            foreach (TypeInfo item in Typ.GetNestedTypes())
            {
                this.TypeInfo(item);
            }
        }

        private void DeclaredTypes()
        {
            this.Log("-Declared Types:");

            foreach (TypeInfo item in Asm.DefinedTypes)
            {
                this.TypeInfo(item);
            }
        }

        /// <summary>
        /// Provides information on the custom attributes of an assembly.
        /// </summary>
        private void CustomAttributes()
        {
            this.Log("-Custom Attributes: ");

            foreach (CustomAttributeData item in Asm.CustomAttributes)
            {
                this.Log("--" + item.AttributeType.FullName);
                this.Log("---" + item.Constructor.Name);
                this.Log("---" + item.Constructor.GetParameters().ToString());
            }
        }

        /// <summary>
        /// Logs the string.
        /// </summary>
        /// <param name="ToLog"></param>
        private void Log(string ToLog)
        {
            Stream.WriteLine(ToLog);
            ConsoleWriter.WriteLine(ToLog);
        }
    }
}