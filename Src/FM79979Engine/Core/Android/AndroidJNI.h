#ifndef _ANDROID_JNI_H_
#define _ANDROID_JNI_H_

#include "jni.h"
//JNIEnv*								cGameApp::m_spThreadEnv = 0;


void

    jclass thisClass = mApp->appThreadEnv->GetObjectClass(mApp->appThreadThis);
	EXCEPTION_RETURN(mApp->appThreadEnv);

	jmethodID showAlertDialog = mApp->appThreadEnv->GetMethodID(thisClass, 
		"showAlertDialog", "()V");
	EXCEPTION_RETURN(mApp->appThreadEnv);

	mApp->appThreadEnv->CallVoidMethod(mApp->appThreadThis, showAlertDialog);
	EXCEPTION_RETURN(mApp->appThreadEnv);

#endif