using CommandEverything.Framework.Util.Text;
using System;
using System.IO;
using System.Reflection;

namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// Holds all sorts of reflection utilities.
    /// </summary>
    public class Reflection
    {
        private StreamWriter Stream;
        private Assembly Asm;

        /// <summary>
        /// Generates data about the specified assembly, and saves it to the specified file.
        /// Also logs the data generated to the console.
        /// </summary>
        /// <param name="File"></param>
        /// <param name="Asm"></param>
        public void GenerateData(StreamWriter File, Assembly Asmm)
        {
            try
            {
                this.Stream = File;
                Asm = Asmm;

                this.Log("-Assembly Location: " + Asmm.CodeBase);
                this.CustomAttributes();
                this.DeclaredTypes();
                this.ReferencedAssemblies();

                try
                {
                    this.Log("-Entry Point: " + Asm.EntryPoint.Name);
                }
                catch (Exception e)
                { }
            }
            catch (Exception e)
            {
                Error.Report(e);
            }
        }

        private void ReferencedAssemblies()
        {
            try
            {
                this.Log("--Referenced Assemblies:");

                foreach (AssemblyName item in Asm.GetReferencedAssemblies())
                {
                    try
                    {
                        this.Log(item.FullName);
                    }
                    catch (Exception e)
                    {
                    }
                    try
                    {
                        this.Log("---" + item.Version.ToString());
                    }
                    catch (Exception e)
                    {
                    }
                    try
                    {
                        this.Log("---" + item.CodeBase.ToString());
                    }
                    catch (Exception e)
                    {
                    }
                }
            }
            catch (Exception TheException)
            {
                Error.Report(TheException);
            }
        }
        
        private unsafe int CalcSize(object obj)
        {
            try
            {
                RuntimeTypeHandle th = obj.GetType().TypeHandle;
                int size = *(*(int**)&th + 1);
                return size;
            }
            catch (Exception TheException)
            {
                Error.Report(TheException);
                return 0;
            }
        }

        private void TypeInfo(TypeInfo Typ)
        {
            try
            {
                this.Log(Typ.FullName);
                this.Log("--Base type: " + Typ.BaseType);
                this.Log("--Type: " + Typ.FullName);
                this.Log("--Sub Types:");

                foreach (Type item in Typ.GetNestedTypes())
                {
                    this.Log("---" + item.FullName);
                    this.TypeInfo(item.GetTypeInfo());
                }

                this.Log("--Type size: " + this.CalcSize(Typ));
                this.Log("**Nested Types:**");

                foreach (TypeInfo item in Typ.GetNestedTypes())
                {
                    this.TypeInfo(item);
                }
            }
            catch (Exception TheException)
            {
                Error.Report(TheException);
            }
        }

        private void DeclaredTypes()
        {
            try
            {
                this.Log("-Declared Types:");

                foreach (TypeInfo item in Asm.DefinedTypes)
                {
                    try
                    {
                        this.TypeInfo(item);
                    }
                    catch (Exception TheException)
                    {
                        //Eat it, cause its probably just a null object.
                    }
                }
            }
            catch (Exception TheException)
            {
                Error.Report(TheException);
            }
        }

        /// <summary>
        /// Provides information on the custom attributes of an assembly.
        /// </summary>
        private void CustomAttributes()
        {
            try
            {
                this.Log("-Custom Attributes: ");

                foreach (CustomAttributeData item in Asm.CustomAttributes)
                {
                    this.Log("--" + item.AttributeType.FullName);
                    this.Log("---" + item.Constructor.Name);
                    this.Log("---" + item.Constructor.GetParameters().ToString());
                }
            }
            catch (Exception TheException)
            {
                Error.Report(TheException);
            }
        }

        /// <summary>
        /// Logs the string.
        /// </summary>
        /// <param name="ToLog"></param>
        private void Log(string ToLog)
        {
            try
            {
                Stream.WriteLine(ToLog);
                ConsoleWriter.WriteLine(ToLog);
            }
            catch (Exception TheException)
            {
                Error.Report(TheException);
            }
        }
    }
}