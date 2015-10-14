// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "actzt598.h"

/////////////////////////////////////////////////////////////////////////////
// CActZt598

IMPLEMENT_DYNCREATE(CActZt598, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CActZt598 properties

CString CActZt598::GetPinValue()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void CActZt598::SetPinValue(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

short CActZt598::GetSerialPort()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}

void CActZt598::SetSerialPort(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}

short CActZt598::GetExtPort()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}

void CActZt598::SetExtPort(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}

short CActZt598::GetTracelevel()
{
	short result;
	GetProperty(0x4, VT_I2, (void*)&result);
	return result;
}

void CActZt598::SetTracelevel(short propVal)
{
	SetProperty(0x4, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CActZt598 operations

short CActZt598::EnableKeyPress()
{
	short result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CActZt598::DisableKeyPress()
{
	short result;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CActZt598::DisablePinPress()
{
	short result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CActZt598::LoadMasterKey(short MkeyNo, LPCTSTR MkeyAsc)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		MkeyNo, MkeyAsc);
	return result;
}

short CActZt598::CloseZt598Com()
{
	short result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CActZt598::OpenZt598Com()
{
	short result;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CActZt598::LoadWorkKey(short MkeyNo, short WKeyNo, LPCTSTR WKeyAsc)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		MkeyNo, WKeyNo, WKeyAsc);
	return result;
}

void CActZt598::GetPortFromIni()
{
	InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short CActZt598::EnablePinPress(short Mode, short PinLen, short MkeyNo, short WKeyNo, LPCTSTR pCardNo, short TimeOut)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_I2 VTS_I2 VTS_BSTR VTS_I2;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Mode, PinLen, MkeyNo, WKeyNo, pCardNo, TimeOut);
	return result;
}

short CActZt598::ResetDevice()
{
	short result;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void CActZt598::AboutVersion()
{
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

short CActZt598::GetCypherPin(VARIANT* chPin)
{
	short result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		chPin);
	return result;
}

short CActZt598::SetDes3Des(short Mode)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Mode);
	return result;
}

short CActZt598::Des(short MkeyNo, short WKeyNo, LPCTSTR DataInput, VARIANT* DataOutput)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		MkeyNo, WKeyNo, DataInput, DataOutput);
	return result;
}

short CActZt598::CalcMacData(short MkeyNo, short WKeyNo, LPCTSTR DataInput, VARIANT* DataOutput)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		MkeyNo, WKeyNo, DataInput, DataOutput);
	return result;
}

short CActZt598::GetVersion(VARIANT* VersionBuf)
{
	short result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		VersionBuf);
	return result;
}

short CActZt598::GetDeviceNo(VARIANT* NOBuf)
{
	short result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		NOBuf);
	return result;
}

short CActZt598::TestCom(VARIANT* chBuf)
{
	short result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		chBuf);
	return result;
}

short CActZt598::SendChar(VARIANT* chBuf, LPCTSTR ch)
{
	short result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_BSTR;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		chBuf, ch);
	return result;
}

short CActZt598::ResetAndSetPin()
{
	short result;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CActZt598::SetKeyboardPrm(short Mode)
{
	short result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Mode);
	return result;
}

short CActZt598::SetDesPra(short p, short f)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		p, f);
	return result;
}

short CActZt598::activeworkkey(short mk, short wk)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		mk, wk);
	return result;
}

short CActZt598::UnDes(short MkeyNo, short WKeyNo, LPCTSTR DataInput, VARIANT* DataOutput)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2 VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		MkeyNo, WKeyNo, DataInput, DataOutput);
	return result;
}

short CActZt598::gDes(LPCTSTR in, LPCTSTR key, VARIANT* out)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		in, key, out);
	return result;
}

short CActZt598::gUndes(LPCTSTR in, LPCTSTR key, VARIANT* out)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x1e, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		in, key, out);
	return result;
}

short CActZt598::gTriDes(LPCTSTR in, LPCTSTR key, VARIANT* out)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		in, key, out);
	return result;
}

short CActZt598::gUnTrides(LPCTSTR in, LPCTSTR key, VARIANT* out)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		in, key, out);
	return result;
}

short CActZt598::ICPowerOn(VARIANT* data)
{
	short result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		data);
	return result;
}

short CActZt598::SetICType(short ic_set, short ic_type)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_I2;
	InvokeHelper(0x22, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		ic_set, ic_type);
	return result;
}

short CActZt598::GetICType(short set, VARIANT* reback)
{
	short result;
	static BYTE parms[] =
		VTS_I2 VTS_PVARIANT;
	InvokeHelper(0x23, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		set, reback);
	return result;
}

short CActZt598::ICControl(LPCTSTR datain, VARIANT* dataout)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x24, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		datain, dataout);
	return result;
}

short CActZt598::ICPowerDown()
{
	short result;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short CActZt598::PinBlockICControl(LPCTSTR datain, VARIANT* dataout)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		datain, dataout);
	return result;
}
