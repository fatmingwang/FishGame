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
    /// Interaction logic for XYZNumeric.xaml
    /// </summary>
    public partial class XYZNumeric : UserControl
    {
        public event EventHandler f_ValueChanged;
        unsafe private float[] m_TempValue = new float[4];
        unsafe private float* m_TempValue2 = null;
        unsafe public XYZNumeric()
        {
            InitializeComponent();
            m_X_numericUpDown.m_Increase_Button.ValueChanged += new RoutedPropertyChangedEventHandler<object>(_ValueChanged);
            m_Y_numericUpDown.m_Increase_Button.ValueChanged += new RoutedPropertyChangedEventHandler<object>(_ValueChanged);
            m_Z_numericUpDown.m_Increase_Button.ValueChanged += new RoutedPropertyChangedEventHandler<object>(_ValueChanged);
            fixed (float* l_pData = m_TempValue)
            {
                m_TempValue2 = l_pData;
            }
        }

        unsafe public float* GetValue()
        {
            if (m_X_numericUpDown.m_Increase_Button.Value != null &&
                m_Y_numericUpDown.m_Increase_Button.Value != null &&
                m_Z_numericUpDown.m_Increase_Button.Value != null)
            {
                m_TempValue2[0] = (float)m_X_numericUpDown.m_Increase_Button.Value;
                m_TempValue2[1] = (float)m_Y_numericUpDown.m_Increase_Button.Value;
                m_TempValue2[2] = (float)m_Z_numericUpDown.m_Increase_Button.Value;
                return m_TempValue2;
            }
            return null;
        }

        void _ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (f_ValueChanged != null)
            {
                f_ValueChanged(sender, e);
            }
        }
        public unsafe void SetValue(float* e_pfValueX, float* e_pfValueY, float* e_pfValueZ)
        {
            this.m_X_numericUpDown.SetValue(e_pfValueX);
            this.m_Y_numericUpDown.SetValue(e_pfValueY);
            this.m_Z_numericUpDown.SetValue(e_pfValueZ);
        }
        public unsafe void SetValue(float* e_pfValue)
        {
            if (e_pfValue != null)
            {
                this.m_X_numericUpDown.SetValue(&e_pfValue[0]);
                this.m_Y_numericUpDown.SetValue(&e_pfValue[1]);
                this.m_Z_numericUpDown.SetValue(&e_pfValue[2]);
            }
            else 
            {
                this.m_X_numericUpDown.SetValue(null);
                this.m_Y_numericUpDown.SetValue(null);
                this.m_Z_numericUpDown.SetValue(null);
            }
        }

        public void SetMaximum(float e_fMaximim)
        {
            this.m_X_numericUpDown.Maximum = e_fMaximim;
            this.m_Y_numericUpDown.Maximum = e_fMaximim;
            this.m_Z_numericUpDown.Maximum = e_fMaximim;
        }

        public void SetMinimum(float e_fMinimim)
        {
            this.m_X_numericUpDown.Minimum = e_fMinimim;
            this.m_Y_numericUpDown.Minimum = e_fMinimim;
            this.m_Z_numericUpDown.Minimum = e_fMinimim;
        }
        public void Increment(float e_Value)
        {
            this.m_X_numericUpDown.Increment = e_Value;
            this.m_Y_numericUpDown.Increment = e_Value;
            this.m_Z_numericUpDown.Increment = e_Value;
        }
    }
}
