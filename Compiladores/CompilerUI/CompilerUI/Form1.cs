using System;
using System.IO;
using System.Security.Permissions;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Windows.Forms;
using System.Diagnostics;
using System.Reflection;
using System.Management;
using System.Resources;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;

namespace CompilerUI
{
    public partial class CompilerUI : Form
    {
        bool            m_isSaved;
        String          m_path;

        dynamic         m_compilerDLL;
        bool            m_isDllLoaded;
        String          m_dllPath;

        String[][]      m_tokens;
        String[]        m_errors;
        String          m_compilationState;

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
                ParseTokenArray(m_compilerDLL.CompileProgram(Editor.Text));

                if (m_tokens != null)
                {
                    for (int i = 0; i < m_tokens.Count(); i++)
                    {
                        dataGridView1.Rows.Add(m_tokens[i]);
                    }

                    for (int i = 0; i < m_errors.Count(); i++)
                    {
                        textBox2.Text += m_errors[i] + System.Environment.NewLine;
                    }
                }
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
                DialogResult wantToSave = MessageBox.Show("No has guardado los cambios, quieres guardar?", "Advertencia", MessageBoxButtons.YesNo);

                if (wantToSave == DialogResult.Yes)
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
            String configuration;

            if (Environment.Is64BitProcess)
            {
                dllArchitecture += "64";
            }
            else
            {
                dllArchitecture += "86";
            }

            if (Assembly.GetEntryAssembly().GetCustomAttributes(false).OfType<DebuggableAttribute>().Any(da => da.IsJITTrackingEnabled))
            {
                dllFilename += dllArchitecture + "d.dll";
                configuration = "Debug";
            }
            else
            {
                configuration = "Release";
                dllFilename += dllArchitecture + ".dll";
            }

            String proccesFullPath = Process.GetCurrentProcess().MainModule.FileName;
            DirectoryInfo Up;

            for (int i = 0; i < 6; i++)
            {
                Up = Directory.GetParent(proccesFullPath);
                proccesFullPath = Up.FullName;
            }

            m_dllPath = proccesFullPath + "\\CompilerUI\\build\\" + dllArchitecture + "\\Exe\\" + configuration + "\\" + dllFilename;

            var DLL = Assembly.UnsafeLoadFrom(m_dllPath);

            var DLLType = DLL.GetType("Compiler.Manager");
            m_compilerDLL = Activator.CreateInstance(DLLType);
            m_isDllLoaded = m_compilerDLL != null;

            if (!m_isDllLoaded)
            {
                MessageBox.Show("Not compiled correctly", dllArchitecture + " dll", MessageBoxButtons.OK);
            }

            return m_isDllLoaded;
        }

        private void ParseTokenArray(String[] tokens)
        {
            if (tokens.Length > 4)
            {
                int index = 0;
                while (tokens[index] != "@")
                {
                    index++;
                }

                if (index > 0)
                {
                    m_compilationState = tokens[0];
                }
                else
                {
                    return;
                }

                m_errors = new String[tokens.Count() - index - 1];
                m_tokens = new String[index - 1][];

                for (int i = 0; i < m_tokens.Count(); i++)
                {
                    String[] token = tokens[i + 1].Split('\r');
                    m_tokens[i] = token;
                }

                for (int i = 0; i < m_errors.Count(); i++)
                {
                    m_errors[i] = tokens[++index];
                }

                dataGridView1.Rows.Clear();
                dataGridView1.Refresh();
                textBox2.Text = m_compilationState + System.Environment.NewLine;
            }
        }

        private void OptionsToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void DataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void VScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            // Create and initialize a VScrollBar.
            VScrollBar vScrollBar1 = new VScrollBar();

            // Dock the scroll bar to the right side of the form.
            vScrollBar1.Dock = DockStyle.Right;

            // Add the scroll bar to the form.
            Controls.Add(vScrollBar1);
        }

        private void DataGridView2_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }
    }
}
