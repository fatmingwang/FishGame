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
    /// Interaction logic for NumericUpDown.xaml
    /// </summary>
    /// 
    public partial class NumericUpDown : UserControl
    {
        public event EventHandler f_ValueChanged;
        unsafe private float[] m_Temp;
        unsafe private float* m_Temp2 = null;
        unsafe public NumericUpDown()
        {
            InitializeComponent();
            m_pfValue = null;
            m_Increase_Button.ValueChanged += new RoutedPropertyChangedEventHandler<object>(_ValueChanged);
            m_Temp = new float[1];
        }

        unsafe void _ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (m_pfValue != null)
            {
                if (Value != null)
                    *m_pfValue = (float)Value;
            }
            if (f_ValueChanged != null)
            {
                f_ValueChanged(sender, e);
            }
        }

        public unsafe void SetValue(float* e_pfValue)
        {
            if (e_pfValue != null)
            {
                m_pfValue = e_pfValue;
                Value = *m_pfValue;
            }
            else 
            {
                m_pfValue = null;
                Value = 0;
            }
        }
        unsafe public float* m_pfValue;
        public double? Maximum { get { return m_Increase_Button.Maximum; } set { m_Increase_Button.Maximum = value; } }
        public double? Increment { get { return m_Increase_Button.Increment; } set { m_Increase_Button.Increment = value; } }
        public double? Minimum { get { return m_Increase_Button.Minimum; } set { m_Increase_Button.Minimum = value; } }
        unsafe public double? Value 
        { 
            get 
            { 
                return m_Increase_Button.Value;
            }
            set 
            { 
                m_Increase_Button.Value = value;
                if (value == null)
                    m_Increase_Button.Value = 0;
                if (m_pfValue != null)
                    *m_pfValue = (float)value;
            }
        }
        unsafe public float* GetValue()
        {
            if (m_Increase_Button.Value != null)
            {
                fixed (float* l_pData = m_Temp)
                {
                    m_Temp2 = l_pData;
                }
                m_Temp2[0] = (float)m_Increase_Button.Value;
                return m_Temp2;
            }
            return null;
        }
    }
}
