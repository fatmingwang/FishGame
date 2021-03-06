namespace UT
{
	extern std::vector<int>	GenerateRandomTable(int e_iCount,int e_iMaxValue);
	extern Vector3	rand(Vector3 e_vMin,Vector3 e_vMax);
	extern Vector2	rand(Vector2 e_vMin,Vector2 e_vMax);
	extern int		rand(int	e_iMin,	int		e_iMax);
	extern float	rand(float	e_fMin,	float	e_fMax);
	extern POINT	rand(POINT	e_Min,	POINT	e_Max);
	//generate a random vector from input data
	template<class T> std::vector<T>	GenerateRandomTable(std::vector<T>*e_pData)
	{
		int	l_Size = (int)e_pData->size();
		std::vector<int>l_RandomTable = GenerateRandomTable(l_Size,l_Size);
		std::vector<T>l_ResultTable;
		l_ResultTable.resize(l_Size);
		for( int i=0;i<l_Size;++i )
		{
			l_ResultTable[i] = ((*e_pData)[l_RandomTable[i]]);
		}
		return l_ResultTable;
	}

	template <class TYPE>inline	TYPE	LERP(TYPE e_fSrc,TYPE e_fDest,float e_fLERPValue )
	{
		return (TYPE)((e_fDest-e_fSrc)*e_fLERPValue+e_fSrc);
	}

	//for each
	//template<class InputIterator, class Function>Function for_each(InputIterator first, InputIterator last, Function f)
	//{
	//	for ( ; first!=last; ++first ) f(*first);
	//		return f;
	//}

	template <class T>	struct	sMinMaxData
	{
		T			Min;
		T			Max;
		//T			Value;
		inline sMinMaxData<T> operator =(Vector2 e_vData)
		{
			sMinMaxData<T>	l_Data = {Min = (T)e_vData.x,Max = (T)e_vData.y};
			return l_Data;
		}
		inline	T	Rand()
		{
			//Value = rand(Min,Max);
			//return Value;
			return rand((T)Min,(T)Max);
		}
	};

	template <class PROBABILITY>struct	sProbabilityVector
	{
		std::vector<PROBABILITY>	ProbabilityVector;
		PROBABILITY					TotalProbabiliy;
		void						SetupProbabilityData(std::vector<PROBABILITY> e_ProbabilityVector)
		{
			ProbabilityVector = e_ProbabilityVector; TotalProbabiliy = 0; for( int i=0;i<(int)ProbabilityVector.size();++i ) TotalProbabiliy += ProbabilityVector[i];
		}
		int							GetIndexByProbability()
		{
			int	l_iSize = (int)ProbabilityVector.size();
			std::vector<int>l_RandomTable = GenerateRandomTable(l_iSize,l_iSize);
			PROBABILITY	l_ProbabilityValue = 0;
			PROBABILITY	l_RandProbabilityValue = rand((PROBABILITY)0,(PROBABILITY)TotalProbabiliy);
			for( int i=0;i<l_iSize;++i )
			{
				l_ProbabilityValue += ProbabilityVector[l_RandomTable[i]];
				if( l_ProbabilityValue >= l_RandProbabilityValue )
					return i;
			}
			return l_iSize-1;
		}
		void						Clear(){ ProbabilityVector.clear(); TotalProbabiliy = 0; }
	};
	//sample
	//struct	sHoldTimeAndType
	//{
	//	UT::sTimeCounter			TC;
	//	eFAICharacterBehaviodStatus	FAICharacterBehaviodStatus;
	//	UT::sMinMaxData<float>		RandHoldTime;
	//};
	//sProbabilityWithValue<float,sHoldTimeAndType>	m_ProbabilityValue;
	//sProbabilityWithValue<float,int>	m_ProbabilityValue;
	template <class PROBABILITY,class VALUE>struct	sProbabilityWithValue
	{
		sProbabilityWithValue(){ TotalProbabiliy = 0; }
		~sProbabilityWithValue(){}
		std::vector<VALUE>			ValueVector;
		std::vector<PROBABILITY>	ProbabilityVector;
		PROBABILITY					TotalProbabiliy;

		void						SetupProbabilityData(std::vector<PROBABILITY> e_ProbabilityVector){ ProbabilityVector = e_ProbabilityVector; TotalProbabiliy = 0; for( int i=0;i<(int)ProbabilityVector.size();++i ) TotalProbabiliy += ProbabilityVector[i];  }
		void						SetupValueData(std::vector<VALUE> e_ValueVector){ ValueVector = e_ValueVector; }
		void						SetupData(std::vector<PROBABILITY> e_ProbabilityVector,std::vector<VALUE> e_ValueVector){SetupProbabilityData(e_ProbabilityVector);SetupValueData(e_ValueVector);}
		int							GetIndexByProbability()
		{
			int	l_iSize = (int)ProbabilityVector.size();
			std::vector<int>l_RandomTable = GenerateRandomTable(l_iSize,l_iSize);
			PROBABILITY	l_ProbabilityValue = 0;
			PROBABILITY	l_RandProbabilityValue = rand((PROBABILITY)0,(PROBABILITY)TotalProbabiliy);
			for( int i=0;i<l_iSize;++i )
			{
				l_ProbabilityValue += ProbabilityVector[l_RandomTable[i]];
				if( l_ProbabilityValue > l_RandProbabilityValue )
					return i;
			}
			return l_iSize-1;
		}
		VALUE*						GetValueByProbability()
		{
			int	l_iIndex = GetIndexByProbability();
			if( l_iIndex != -1 )
			{
				return &ValueVector[l_iIndex];
			}
			return 0;
		}
		void						Clear(){ ProbabilityVector.clear(); TotalProbabiliy = 0; ValueVector.clear(); }
	};
//end namespace UT
}