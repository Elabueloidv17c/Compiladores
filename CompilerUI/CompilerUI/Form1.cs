using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;
using System.Resources;
using System.Diagnostics;
using System.Management;

//Leer patron de máquina de estados

namespace CompilerUI
{
    public partial class CompilerUI : Form
    {
        bool            m_isSaved;
        String          m_path;

        dynamic         m_compilerDLL;
        bool            m_isDllLoaded;
        String          m_dllPath;

        public CompilerUI()
        {
            InitializeComponent();

            m_dllPath = "";
            m_isDllLoaded = false;

            m_path = "";
            m_isSaved = false;
        }

        private void NewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Save();
            Editor.Text = "";
            m_isSaved = false;
        }

        private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Save();
            Open();
        }

        private void SaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_isSaved)
            {
                System.IO.File.WriteAllText(@m_path, Editor.Text);
            }
            else
            {
                SaveFile();
            }
        }

        private void SaveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFile();
        }

        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Save();

            System.Windows.Forms.Application.Exit();
        }

        private void CompilerUI_Load(object sender, EventArgs e)
        {
            if(!LoadCompilerDLL())
            {
                MessageBox.Show("DLL Not loaded", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            m_isDllLoaded = true;
        }

        private void CompileProgramToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_isDllLoaded)
            {
                m_compilerDLL.CompileProgram(Editor.Text);
            }
            else
            {
                MessageBox.Show("DLL Not loaded", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void SaveFile()
        {
            Stream myStream;
            SaveFileDialog Window = new SaveFileDialog();

            Window.Filter = "CompilerFunkAlux Files (*.cfa)|*.cfa";
            Window.FilterIndex = 2;
            Window.RestoreDirectory = true;

            if (Window.ShowDialog() == DialogResult.OK)
            {
                if ((myStream = Window.OpenFile()) != null)
                {
                    m_path = Path.GetFullPath(Window.FileName);
                    myStream.Close();

                    System.IO.File.WriteAllText(@m_path, Editor.Text);
                    m_isSaved = true;
                }
            }
        }

        private void Save()
        {
            if (Edited())
            {
                if (MessageBox.Show("No has guardado los cambios, quieres guardar?", "Advertencia", MessageBoxButtons.YesNo) != 0)
                {
                    SaveFile();
                    return;
                }
            }
        }

        private void Open()
        {
            Stream myStream;
            OpenFileDialog Window = new OpenFileDialog();

            Window.Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
            Window.FilterIndex = 2;
            Window.RestoreDirectory = true;

            if (Window.ShowDialog() == DialogResult.OK)
            {
                if ((myStream = Window.OpenFile()) != null)
                {
                    m_path = Path.GetFullPath(Window.FileName);
                    myStream.Close();

                    Editor.Text = System.IO.File.ReadAllText(@m_path);
                    m_isSaved = true;
                }
            }
        }

        private bool Edited()
        {
            if(m_isSaved)
            {
                String savedFile = System.IO.File.ReadAllText(@m_path);

                if (savedFile != Editor.Text)
                {
                    return true;
                }
            }
            else
            {
                if(Editor.Text != "")
                {
                    return true;
                }
            }

            return false;
        }

        private bool LoadCompilerDLL()
        {
            String dllFilename = "Compiler";
            String dllArchitecture = "x";

            if (Environment.Is64BitProcess)
            {
                dllArchitecture += "64";
            }
            else
            {
                dllArchitecture += "86";
            }

            if (System.Diagnostics.Debugger.IsAttached)
            {
                dllFilename += dllArchitecture + "d.dll";
            }
            else
            {
                dllFilename += dllArchitecture + ".dll";
            }

            String proccesFullPath = Process.GetCurrentProcess().MainModule.FileName; //Solo 64 bits
            DirectoryInfo Up;

            for (int i = 0; i < 6; i++)
            {
                Up = Directory.GetParent(proccesFullPath);
                proccesFullPath = Up.FullName;
            }

            m_dllPath = proccesFullPath + "\\Compiler\\" + dllArchitecture + "\\Exe\\Debug\\" + dllFilename;

            var DLL = Assembly.UnsafeLoadFrom(m_dllPath);

            var DLLType = DLL.GetType("Compiler.Manager");
            m_compilerDLL = Activator.CreateInstance(DLLType);
            m_isDllLoaded = m_compilerDLL != null;

            if (m_isDllLoaded)
            {
                MessageBox.Show("Compiled correctly", dllArchitecture + " dll", MessageBoxButtons.OK);
            }
            else
            {
                MessageBox.Show("Not compiled correctly", dllArchitecture + " dll", MessageBoxButtons.OK);
            }

            return m_isDllLoaded;
        }
    }
}
