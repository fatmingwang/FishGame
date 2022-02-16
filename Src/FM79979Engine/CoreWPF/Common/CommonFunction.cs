using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Navigation;
using System.Windows.Media;
using System.Windows.Markup;
using System.Xml;
namespace CoreWPF.Common
{
    class CommonFunction
    {
        public static T Clone<T>(T from)
        {
            //// Create the Button.
            //Button origianlButton = new Button();
            //origianlButton.Height = 50;
            //origianlButton.Width = 100;
            //origianlButton.Background = Brushes.AliceBlue;
            //origianlButton.Content = "Click Me";

            //// Save the Button to a string.
            //string savedButton = XamlWriter.Save(origianlButton);

            //// Load the button
            //StringReader stringReader = new StringReader(savedButton);
            //XmlReader xmlReader = XmlReader.Create(stringReader);
            //Button readerLoadButton = (Button)XamlReader.Load(xmlReader);
            string objStr = XamlWriter.Save(from);
            StringReader stringReader = new StringReader(objStr);
            XmlReader xmlReader = XmlReader.Create(stringReader);
            object clone = XamlReader.Load(xmlReader);
            return (T)clone;
        }
        //private TreeViewItem CreateTreeViewItem(string header,string iconFolder,string iconName)
        public TreeViewItem CreateTreeViewItem(string header,string iconFolder,string iconName)
        {
            TreeViewItem child = new TreeViewItem();
            StackPanel pan = new StackPanel();
            if (iconName != null)
            {
                pan.Orientation = Orientation.Horizontal;
                System.Windows.Media.Imaging.IconBitmapDecoder icon = new System.Windows.Media.Imaging.IconBitmapDecoder(new Uri(System.IO.Path.Combine(iconFolder, iconName), UriKind.RelativeOrAbsolute), System.Windows.Media.Imaging.BitmapCreateOptions.None, System.Windows.Media.Imaging.BitmapCacheOption.OnLoad);
                //IconBitmapDecoder icon = new IconBitmapDecoder(m_VisibleImage.StreamSource, BitmapCreateOptions.None, BitmapCacheOption.OnLoad);
                Image image = new Image();
                image.Height = 16;
                image.Source = icon.Frames[0];
                pan.Children.Add(image);
            }
            pan.Children.Add(new TextBlock(new Run("  " + header)));
            child.Header = pan;
            return child;
        }

        static public void DragEnter(object sender, DragEventArgs e)
        {
            bool isCorrect = true;
            if (e.Data.GetDataPresent(DataFormats.FileDrop, true) == true)
            {
                string[] filenames = (string[])e.Data.GetData(DataFormats.FileDrop, true);
                foreach (string filename in filenames)
                {
                    if (File.Exists(filename) == false)
                    {
                        isCorrect = false;
                        break;
                    }
                    FileInfo info = new FileInfo(filename);
                    if (info.Extension != ".avi")
                    {
                        isCorrect = false;
                        break;
                    }
                }
            }
            if (isCorrect == true)
                e.Effects = DragDropEffects.All;
            else
                e.Effects = DragDropEffects.None;
            e.Handled = true;
        }

        static public void JumpToNode(TreeViewItem tvi, string NodeName)
        {
            if (tvi.Name == NodeName)
            {
                tvi.IsExpanded = true;
                tvi.BringIntoView();
                return;
            }
            else
                tvi.IsExpanded = false;

            if (tvi.HasItems)
            {
                foreach (var item in tvi.Items)
                {
                    TreeViewItem temp = item as TreeViewItem;
                    JumpToNode(temp, NodeName);
                }
            }
        }
    }
}
