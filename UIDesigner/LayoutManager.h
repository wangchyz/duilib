#pragma once
#include "..\DuiLib\UIlib.h"
using DuiLib::CContainerUI;
using DuiLib::CPaintManagerUI;
using DuiLib::CWindowWnd;
using DuiLib::TNotifyUI;

//#define EXPORT_UI_SCRIPT

#if defined(EXPORT_UI_SCRIPT)
#include "tinyxml.h"
#endif

//////////////////////////////////////////////////////////////////////////
//CFormUI

class CFormUI : public CContainerUI
{
public:
	CFormUI();
	virtual ~CFormUI();

public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetManager(CPaintManagerUI* pPaintManager);
	CPaintManagerUI* GetManager() const;

	SIZE GetInitSize();
	void SetInitSize(int cx, int cy);
	RECT& GetSizeBox();
	void SetSizeBox(RECT& rcSizeBox);
	RECT& GetCaptionRect();
	void SetCaptionRect(RECT& rcCaption);
	SIZE GetRoundCorner() const;
	void SetRoundCorner(int cx, int cy);
	SIZE GetMinMaxInfo() const;
	void SetMinMaxInfo(int cx, int cy);
	bool IsShowUpdateRect() const;
	void SetShowUpdateRect(bool show);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void SetPos(RECT rc);

private:
	CPaintManagerUI* m_pManager;
};

//////////////////////////////////////////////////////////////////////////
//CFormTestWnd

class CFormTestWnd :  public CWindowWnd, public INotifyUI
{
public:
	CFormTestWnd();
	~CFormTestWnd();
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);

public:
	void SetManager(CPaintManagerUI* pPaintManager);
	CPaintManagerUI* GetManager() const;
	void SetRoot(CControlUI* pControl);

	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CPaintManagerUI* m_pManager;
	CControlUI* m_pRoot;
};

//////////////////////////////////////////////////////////////////////////
//CDelayResize

class CDelayRepos
{
public:
	CDelayRepos();
	~CDelayRepos();

public:
	BOOL AddParent(CControlUI* pControl);
	void Repos();

private:
	CArray<CControlUI*,CControlUI*> m_arrDelay;
};

//////////////////////////////////////////////////////////////////////////
//CLayoutManager

class CLayoutManager
{
public:
	CLayoutManager(void);
	virtual ~CLayoutManager(void);

public:
	void Init(HWND hWnd,LPCTSTR pstrLoad);

	void Draw(CDC* pDC);
	void DrawAuxBorder(CDC* pDC,CControlUI* pControl);

	static CControlUI* NewControl(int nClass,CRect& rect,CPaintManagerUI* pManager,CControlUI* pParent);
	BOOL RemoveControl(CControlUI* pControl);
	CFormUI* GetForm() const;
	CControlUI* FindControl(CPoint point) const;

	void TestForm();
	BOOL IsEmptyForm() const;
	void AlignLeft(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignRight(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignTop(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignBottom(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignCenterVertically(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignCenterHorizontally(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignHorizontal(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignVertical(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignSameWidth(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignSameHeight(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void AlignSameSize(CControlUI* pFocused,CArray<CControlUI*,CControlUI*>& arrSelected);
	void ShowGrid(bool bShow);
	bool IsShowGrid() const;
	void ShowAuxBorder(bool bShow);
	bool IsShowAuxBorder() const;
	void MicoMoveUp(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);
	void MicoMoveDown(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);
	void MicoMoveLeft(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);
	void MicoMoveRight(CArray<CControlUI*,CControlUI*>& arrSelected,int nMoved);

protected:
	CControlUI* CopyControls(CControlUI* pControl);
	CControlUI* CopyControl(CControlUI* pControl);

#if defined(EXPORT_UI_SCRIPT)
	void SaveSkinFile();

	void SaveProperties(CControlUI* pControlUI, TiXmlElement* pParentNode);
	void SaveSingleProperties(CControlUI* pControlUI, TiXmlElement* pParentNode);
	void SaveContainerProperties(CControlUI* pControlUI, TiXmlElement* pParentNode);

	void CControlUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CLabelUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CButtonUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void COptionUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CProgressUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CSliderUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CEditUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CScrollbarUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CListUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CComboUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CListHeaderItemUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
	void CListElementUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);

	void CContainerUI_Properties(CControlUI* pControlUI, TiXmlElement* pNode);
#endif

private:
	CPaintManagerUI m_Manager;

	bool m_bShowGrid;
	bool m_bShowAuxBorder;
};