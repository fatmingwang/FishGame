using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CoreWPF.Common
{
    /// <summary>
    /// EditableListbox.xaml
    /// 
    /// listbox will add a textbox as its item
    /// </summary>
    public partial class EditableListbox : UserControl
    {
        string m_TextStartEditText;
        public bool m_bSameItemAddAlow = false;
        TextBox m_EditedTextBox = null;
        public event EventHandler f_DeleteSelectedItem;
        public event EventHandler f_AddItem;
        public event EventHandler f_Replace;
        public event EventHandler f_ChangeName;
        public EditableListbox()
        {
            InitializeComponent();
            this.m_ListBox.KeyUp += new System.Windows.Input.KeyEventHandler(EditableListbox_KeyUp);
            this.m_ListBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(EditableListbox_MouseDoubleClick);
            this.m_ListBox.SelectionMode = System.Windows.Controls.SelectionMode.Extended;
        }

        public int IndexOf(string e_strName)
        {
            int l_iIndex = 0;
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    return l_iIndex;
                ++l_iIndex;
            }
            return -1;
        }
        public bool IsContain(string e_strName)
        {
            foreach(TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    return true;
            }
            return false;
        }

        public int NumSame(string e_strName)
        {
            int l_iValue = 0;
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.Text.CompareTo(e_strName) == 0)
                    ++l_iValue;
            }
            return l_iValue;
        }

        public bool AddListBoxItem(string e_strName)
        {
            if (!m_bSameItemAddAlow)
            {
                if (IsContain(e_strName))
                {
                    if (f_Replace != null)
                    {
                        if (MessageBox.Show("replace exists!?", "Warning", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                        {
                            int l_iIndex = IndexOf(e_strName);
                            f_Replace(m_ListBox.Items[l_iIndex], new RoutedEventArgs());
                        }
                    }
                    return false;
                }
            }
            TextBox l_TextBox = new TextBox();
            l_TextBox.Text = e_strName;
            l_TextBox.IsEnabled = false;
            //l_TextBox.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
            //l_TextBox.MouseDoubleClick += new System.Windows.Input.MouseButtonEventHandler(EditableListbox_MouseDoubleClick);
            m_ListBox.Items.Add(l_TextBox);
            m_EditedTextBox = null;
            if (f_AddItem != null)
            { 
                System.EventArgs e = new EventArgs();
                f_AddItem(l_TextBox, e);
            }
            return true;
        }

        void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            TextBox l_TextBox = sender as TextBox;
            if (m_EditedTextBox != l_TextBox)
            {
                MessageBox.Show("error TextBox_LostFocus");
            }
            l_TextBox.LostFocus -= new RoutedEventHandler(TextBox_LostFocus);
            if (NumSame(l_TextBox.Text) != 1 && m_bSameItemAddAlow != true)
            {
                MessageBox.Show("same name", "please change name", MessageBoxButton.OK);
                l_TextBox.Text = m_TextStartEditText;
            }
            else
            {
                if (f_ChangeName != null)
                    f_ChangeName(sender, e);
            }
            l_TextBox.IsEnabled = false;
            m_EditedTextBox = null;
        }

        public void DeleteSelectedItems()
        {
            while (this.m_ListBox.SelectedItems.Count>0)
            {
                if (null != f_DeleteSelectedItem)
                {
                    System.EventArgs e = new EventArgs();
                    f_DeleteSelectedItem(this.m_ListBox.SelectedItems[0], e);
                }
                this.m_ListBox.Items.RemoveAt(this.m_ListBox.Items.IndexOf(this.m_ListBox.SelectedItems[0]));
            }
        }

        public bool IsInEditMode()
        {
            foreach (TextBox l_TextBox in this.m_ListBox.Items)
            {
                if (l_TextBox.IsEnabled == true)
                    return true;
            }
            return false;
        }

        public string SelectedName()
        {
            if (m_ListBox.SelectedItem != null)
            {
                TextBox l_SelectedTextBox = m_ListBox.SelectedItem as TextBox;
                return l_SelectedTextBox.Text;
            }
            return null;
        }

        void ChangeTextBoxName(TextBox sender)
        {
            if (sender.IsEnabled == false && m_EditedTextBox == null)
            {
                m_EditedTextBox = sender;
                sender.IsEnabled = true;
                sender.LostFocus += new RoutedEventHandler(TextBox_LostFocus);
                m_TextStartEditText = sender.Text;
                sender.SelectAll();
                sender.Focus();
            }
        }

        void EditableListbox_KeyUp(System.Object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (this.m_ListBox.SelectedItem != null)
            {
                TextBox l_SelectedTextBox = m_ListBox.SelectedItem as TextBox;
                if (e.Key == Key.Enter)
                {
                    if ( NumSame(l_SelectedTextBox.Text) == 1 || m_bSameItemAddAlow == true )
                    {
                        l_SelectedTextBox.IsEnabled = false;
                        l_SelectedTextBox.LostFocus -= new RoutedEventHandler(TextBox_LostFocus);
                        m_EditedTextBox = null;
                        if (m_TextStartEditText.CompareTo(l_SelectedTextBox.Text) != 0)
                        {
                            if (f_ChangeName != null)
                                f_ChangeName(l_SelectedTextBox, e);
                        }
                        m_TextStartEditText = null;
                    }
                    else
                    {
                        MessageBox.Show("same name", "please change name", MessageBoxButton.OK);
                    }
                }
                else
                if (e.Key == Key.F2)
                {
                    ChangeTextBoxName(l_SelectedTextBox);
                }
            }
        }

        void EditableListbox_MouseDoubleClick(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (this.m_ListBox.SelectedItem != null)
            {
                ChangeTextBoxName(this.m_ListBox.SelectedItem as TextBox);
            }
        }
    }
}
