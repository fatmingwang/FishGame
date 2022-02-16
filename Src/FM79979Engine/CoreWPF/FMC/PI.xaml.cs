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

namespace CoreWPF.FMC
{
    /// <summary>
    /// Interaction logic for PI.xaml
    /// </summary>
    public partial class PI : UserControl
    {
        public event EventHandler f_AddPI;
        public event EventHandler f_DelPI;
        public event EventHandler f_PIChangeSelection;
        public event EventHandler f_PIUnitChangeSelection;

        public PI()
        {
            InitializeComponent();
        }
        private void AddPI_Click(object sender, RoutedEventArgs e)
        {
            if (f_AddPI != null)
            {
                f_AddPI(sender, e);
            }
        }

        private void DelPI_Click(object sender, RoutedEventArgs e)
        {
            if (f_DelPI != null)
            {
                f_DelPI(sender, e);
            }
        }

        private void PIList_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            if (f_PIChangeSelection != null)
            {
                f_PIChangeSelection(sender, args);
            }
        }

        private void PIUnit_ListboxChangeSelection(object sender, SelectionChangedEventArgs args)
        {
            if (f_PIUnitChangeSelection != null)
            {
                f_PIUnitChangeSelection(sender, args);
            }
        }

        private void m_PIUnit_listBox_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            object item = m_PIUnit_listBox.SelectedItem;
            if (item != null)
                DragDrop.DoDragDrop(m_PIUnit_listBox, item, DragDropEffects.Move);
        }

    }
}
