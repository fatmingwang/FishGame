#ifndef _OBJECT_STATUS_WITH_DATA_H_
#define _OBJECT_STATUS_WITH_DATA_H_

namespace FATMING_CORE
{

	//T must have
	class	cObjectStatusWithData:public NamedTypedObject
	{
	protected:
		NamedTypedObject	*m_pObject;
	public:
		cObjectStatusWithData(WCHAR*e_strStatusName)
		{
			this->SetName(e_strStatusName);
		}
	};
//endnamespace FATMING_CORE
}

#endif