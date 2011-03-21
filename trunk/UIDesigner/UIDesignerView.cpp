// UIDesignerView.cpp : CUIDesignerView ���ʵ��
//

#include "stdafx.h"
#include "UIDesigner.h"

#include "UIDesignerDoc.h"
#include "UIDesignerView.h"

#include "PropertyTabLayoutUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CUIDesignerView

CLIPFORMAT CUIDesignerView::m_cfUI=(CLIPFORMAT)::RegisterClipboardFormat(_T("UIDesigner Clipboard Data Format"));

IMPLEMENT_DYNCREATE(CUIDesignerView, CScrollView)

BEGIN_MESSAGE_MAP(CUIDesignerView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CUIDesignerView::OnFilePrintPreview)
	ON_COMMAND(ID_FORMEDIT_TEST, &CUIDesignerView::OnFormEditTest)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_TEST, &CUIDesignerView::OnUpdateFormEditTest)
	ON_COMMAND(ID_FORMEDIT_ALIGN_LEFT, &CUIDesignerView::OnAlignLeft)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_LEFT, &CUIDesignerView::OnUpdateAlignLeft)
	ON_COMMAND(ID_FORMEDIT_ALIGN_RIGHT, &CUIDesignerView::OnAlignRight)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_RIGHT, &CUIDesignerView::OnUpdateAlignRight)
	ON_COMMAND(ID_FORMEDIT_ALIGN_TOP, &CUIDesignerView::OnAlignTop)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_TOP, &CUIDesignerView::OnUpdateAlignTop)
	ON_COMMAND(ID_FORMEDIT_ALIGN_BOTTOM, &CUIDesignerView::OnAlignBottom)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_BOTTOM, &CUIDesignerView::OnUpdateAlignBottom)
	ON_COMMAND(ID_FORMEDIT_ALIGN_CENTER_VERTICALLY, &CUIDesignerView::OnAlignCenterVertically)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_CENTER_VERTICALLY, &CUIDesignerView::OnUpdateAlignCenterVertically)
	ON_COMMAND(ID_FORMEDIT_ALIGN_CENTER_HORIZONTALLY, &CUIDesignerView::OnAlignCenterHorizontally)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_CENTER_HORIZONTALLY, &CUIDesignerView::OnUpdateAlignCenterHorizontally)
	ON_COMMAND(ID_FORMEDIT_ALIGN_HORIZONTAL, &CUIDesignerView::OnAlignHorizontal)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_HORIZONTAL, &CUIDesignerView::OnUpdateAlignHorizontal)
	ON_COMMAND(ID_FORMEDIT_ALIGN_VERTICAL, &CUIDesignerView::OnAlignVertical)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_VERTICAL, &CUIDesignerView::OnUpdateAlignVertical)
	ON_COMMAND(ID_FORMEDIT_ALIGN_SAME_WIDTH, &CUIDesignerView::OnAlignSameWidth)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_SAME_WIDTH, &CUIDesignerView::OnUpdateAlignSameWidth)
	ON_COMMAND(ID_FORMEDIT_ALIGN_SAME_HEIGHT, &CUIDesignerView::OnAlignSameHeight)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_SAME_HEIGHT, &CUIDesignerView::OnUpdateAlignSameHeight)
	ON_COMMAND(ID_FORMEDIT_ALIGN_SAME_SIZE, &CUIDesignerView::OnAlignSameSize)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_ALIGN_SAME_SIZE, &CUIDesignerView::OnUpdateAlignSameSize)
	ON_COMMAND(ID_FORMEDIT_SHOW_GRID, &CUIDesignerView::OnShowGrid)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_SHOW_GRID, &CUIDesignerView::OnUpdateShowGrid)
	ON_COMMAND(ID_FORMEDIT_SHOW_AUXBORDER, &CUIDesignerView::OnShowAuxBorder)
	ON_UPDATE_COMMAND_UI(ID_FORMEDIT_SHOW_AUXBORDER, &CUIDesignerView::OnUpdateShowAuxBorder)
	ON_COMMAND(ID_EDIT_CUT, &CUIDesignerView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CUIDesignerView::OnUpdateNeedSel)
	ON_COMMAND(ID_EDIT_COPY, &CUIDesignerView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CUIDesignerView::OnUpdateNeedSel)
	ON_COMMAND(ID_EDIT_PASTE, &CUIDesignerView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CUIDesignerView::OnUpdateNeedClip)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CUIDesignerView ����/����

CUIDesignerView::CUIDesignerView()
{
	// TODO: �ڴ˴����ӹ������
	m_ptDPtoLP.x=0;
	m_ptDPtoLP.y=0;
	m_bInit=false;
}

CUIDesignerView::~CUIDesignerView()
{
	ReleaseExtendedAttributes(m_LayoutManager.GetForm());
}

BOOL CUIDesignerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CUIDesignerView ����

void CUIDesignerView::OnDraw(CDC* pDrawDC)
{
	CUIDesignerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ�����������ӻ��ƴ���
	CMemDC memDC(*pDrawDC, this);
	CDC* pDC = &memDC.GetDC();

	CRect rectClient;
	GetClientRect(rectClient);
	CPoint point=GetScrollPosition();
	rectClient.OffsetRect(point);
	pDC->FillSolidRect(rectClient,RGB(255, 255, 255));

	CSize szFormSize=m_LayoutManager.GetForm()->GetInitSize();
	CSize szFormOffset(FORM_OFFSET_X,FORM_OFFSET_Y);
	CDC hCloneDC;
	HBITMAP hNewBitmap;
	hCloneDC.CreateCompatibleDC(pDC);
	hNewBitmap=::CreateCompatibleBitmap(pDC->GetSafeHdc(),szFormSize.cx,szFormSize.cy);
	HBITMAP hOldBitmap=(HBITMAP)hCloneDC.SelectObject(hNewBitmap);
	m_LayoutManager.Draw(&hCloneDC);
	pDC->BitBlt(szFormOffset.cx,szFormOffset.cy,szFormSize.cx,szFormSize.cy,&hCloneDC,0,0,SRCCOPY);
	hCloneDC.SelectObject(hOldBitmap);
	::DeleteDC(hCloneDC);
	::DeleteObject(hNewBitmap);

	m_MultiTracker.Draw(pDC,&szFormOffset);
}


// CUIDesignerView ��ӡ
void CUIDesignerView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CUIDesignerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CUIDesignerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CUIDesignerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ���Ӵ�ӡ����е���������
}

void CUIDesignerView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUIDesignerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CUIDesignerView ���

#ifdef _DEBUG
void CUIDesignerView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CUIDesignerView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CUIDesignerDoc* CUIDesignerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIDesignerDoc)));
	return (CUIDesignerDoc*)m_pDocument;
}
#endif //_DEBUG


// CUIDesignerView ��Ϣ��������

void CUIDesignerView::OnInitialUpdate()
{
	__super::OnInitialUpdate();

	// TODO: �ڴ�����ר�ô����/����û���
	CUIDesignerDoc* pDoc=GetDocument();

	m_LayoutManager.Init(this->GetSafeHwnd(),pDoc->GetPathName());
	CFormUI* pForm=m_LayoutManager.GetForm();

	g_pClassView->InsertUITreeItem(pForm,pDoc->GetTitle());
	if(pForm->GetCount()>0)
		InitUI(pForm->GetItemAt(0));
	g_pClassView->SelectUITreeItem(pForm);
	m_bInit=true;

	m_MultiTracker.SetHandleSize(TRACKER_HANDLE_SIZE);
	m_MultiTracker.SetStyle(dottedLine|resizeOutside);
	m_MultiTracker.SetFormSize(pForm->GetInitSize());
	m_MultiTracker.Add(CreateTracker(pForm));

	SetScrollSizes(MM_TEXT,CSize(FORM_INIT_WIDTH+80,FORM_INIT_HEIGHT+80));
}

void CUIDesignerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	CClientDC dc(this);
	OnPrepareDC(&dc);//Device coordinates to Logical coordinates
	dc.SetWindowOrg(-FORM_OFFSET_X,-FORM_OFFSET_Y);//Logical coordinates to Form coordinates

	CPoint ptLogical=point-m_ptDPtoLP;//Device coordinates to Logical coordinates
	ptLogical.Offset(-FORM_OFFSET_X,-FORM_OFFSET_Y);//Logical coordinates to Form coordinates

	CControlUI* pControl=m_LayoutManager.FindControl(ptLogical);
	CTrackerElement* pTracker=NULL;
	if(pControl==NULL)
		pControl=m_LayoutManager.GetForm();

	int nHit=m_MultiTracker.HitTest(ptLogical);
	int nType=GetControlType(pControl);
	if((nFlags&MK_CONTROL)==0&&nHit==hitNothing)
		m_MultiTracker.RemoveAll();
	if(nHit==hitNothing)
		m_MultiTracker.Add(CreateTracker(pControl));
	else
		m_MultiTracker.SetFocus(ptLogical);

	if(nHit>=0||nType==typeControl)
	{
		m_MultiTracker.Track(this, ptLogical, FALSE,&dc);
	}
	else
	{
		CControlTracker tracker;
		int nClass=g_pToolBoxWnd->GetCurSel()->GetClass();
		CRect rect;
		if (tracker.TrackRubberBand(this, point, TRUE))
		{
			rect=tracker.GetRect();
			rect.NormalizeRect();
			rect.OffsetRect(-FORM_OFFSET_X,-FORM_OFFSET_Y);
			if(rect.Width()<10&&rect.Height()<10)
				rect.SetRect(ptLogical.x,ptLogical.y,ptLogical.x+UI_DEFAULT_WIDTH,ptLogical.y+UI_DEFAULT_HEIGHT);
		}
		else
		{
			rect.SetRect(ptLogical.x,ptLogical.y,ptLogical.x+UI_DEFAULT_WIDTH,ptLogical.y+UI_DEFAULT_HEIGHT);
		}

		if(nClass>classPointer)
		{
			CControlUI* pNewControl=CLayoutManager::NewControl(nClass,rect,pControl->GetManager(),pControl);
			g_pClassView->InsertUITreeItem(pNewControl);
			g_pToolBoxWnd->SetCurSel(classPointer);

			m_MultiTracker.RemoveAll();
			m_MultiTracker.Add(CreateTracker(pNewControl));
		}
	}

	g_pClassView->SelectUITreeItem(m_MultiTracker.GetFocused());
	if(m_MultiTracker.GetSize()==1)
		g_pPropertiesWnd->ShowProperty(m_MultiTracker.GetFocused());
	else
		g_pPropertiesWnd->HideAllProperties(TRUE,TRUE);
	
	this->Invalidate(FALSE);

// 	__super::OnLButtonDown(nFlags, point);
}

BOOL CUIDesignerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	CPoint point;
	GetCursorPos(&point);
	this->ScreenToClient(&point);
	point-=m_ptDPtoLP;
	point.Offset(-FORM_OFFSET_X,-FORM_OFFSET_Y);

	if (pWnd==this&&m_MultiTracker.SetCursor(point,nHitTest))
		return TRUE;

	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CUIDesignerView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: �ڴ˴�������Ϣ�����������
	if(m_bInit==false)
		return;

	SIZE size=m_LayoutManager.GetForm()->GetInitSize();
	SetScrollSizes(MM_TEXT,CSize(size.cx+FORM_OFFSET_X+80,
		size.cy+FORM_OFFSET_Y+80));

	UpDateDPtoLPOffset();
}

void CUIDesignerView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	UpDateDPtoLPOffset();

	__super::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CUIDesignerView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	UpDateDPtoLPOffset();

	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}

CTrackerElement* CUIDesignerView::CreateTracker(CControlUI* pControl)
{
	CTrackerElement* pTracker=NULL;
	int nType=GetControlType(pControl);
	if(nType==typeForm)
		pTracker=new CTrackerElement(pControl,typeForm,this);
	else if(nType==typeContainer)
		pTracker=new CTrackerElement(pControl,typeContainer,this);
	else
		pTracker=new CTrackerElement(pControl,typeControl,this);

	return pTracker;
}

int CUIDesignerView::GetControlType(CControlUI* pControl)
{
	if(pControl->GetInterface(_T("Form")))
		return typeForm;
	else if(pControl->GetInterface(_T("Container")))
		return typeContainer;

	return typeControl;
}

void CUIDesignerView::Notify(TNotifyUI& msg)
{
	CControlUI* pControl=msg.pSender;

	if(msg.sType==_T("setpos"))
	{
		if(GetControlType(pControl)==typeForm)
		{
			CFormUI* pForm=m_LayoutManager.GetForm();
			SIZE size;
			size.cx=pForm->GetFixedWidth();
			size.cy=pForm->GetFixedHeight();
			pForm->SetInitSize(size.cx,size.cy);

			g_pPropertiesWnd->SetPropValue(pControl,tagFormSize);
			SetScrollSizes(MM_TEXT,CSize(size.cx+FORM_OFFSET_X+80,
				size.cy+FORM_OFFSET_Y+80));
			m_MultiTracker.SetFormSize(size);
		}
		else
		{
			CControlUI* pParent=pControl->GetParent();
			if(pParent)
				pParent->SetPos(pParent->GetPos());

			g_pPropertiesWnd->SetPropValue(pControl,tagPos);
			g_pPropertiesWnd->SetPropValue(pControl,tagSize);
		}
	}
	else if(msg.sType==_T("formsize"))
	{
		if(GetControlType(pControl)!=typeForm)
			return;
		CFormUI* pForm=m_LayoutManager.GetForm();
		SIZE size=pForm->GetInitSize();

		pForm->SetFixedWidth(size.cx);
		pForm->SetFixedHeight(size.cy);

		SetScrollSizes(MM_TEXT,CSize(size.cx+FORM_OFFSET_X+80,
			size.cy+FORM_OFFSET_Y+80));
		m_MultiTracker.SetFormSize(size);
	}
}

void CUIDesignerView::UpDateDPtoLPOffset()
{
	if(m_nMapMode==0)
		return;

	CPoint point(0,0);
	CClientDC dc(this);
	OnPrepareDC(&dc,NULL);
	dc.LPtoDP(&point);
	m_ptDPtoLP=point;
}
BOOL CUIDesignerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ

	return TRUE;
}

void CUIDesignerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	UpDateDPtoLPOffset();

	__super::OnMouseMove(nFlags, point);
}

void CUIDesignerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_RETURN:
		OnShowPropertyDialog();
		break;
	case VK_DELETE:
		OnRemoveControl();
		break;
	case VK_UP:
		OnMicoMoveUp();
		break;
	case VK_DOWN:
		OnMicoMoveDown();
		break;
	case VK_LEFT:
		OnMicoMoveLeft();
		break;
	case VK_RIGHT:
		OnMicoMoveRight();
		break;
	}

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CUIDesignerView::OnRemoveControl()
{
	CControlUI* pControl=m_MultiTracker.GetFocused();
	if(pControl==m_LayoutManager.GetForm())
		return FALSE;

	CControlUI* pParent=pControl->GetParent();
	HTREEITEM hDelete=(HTREEITEM)(((ExtendedAttributes*)pControl->GetTag())->hItem);
	BOOL bRet=m_LayoutManager.RemoveControl(pControl);
	if(bRet)
	{
		g_pPropertiesWnd->HideAllProperties(TRUE,TRUE);
		m_MultiTracker.RemoveAll();
		g_pClassView->RemoveUITreeItem(hDelete);
		if(pParent)
			pParent->NeedUpdate();
	}

	return bRet;
}

void CUIDesignerView::SelectControl(CControlUI* pControl)
{
	if(pControl==NULL||pControl==m_MultiTracker.GetFocused())
		return;

	CControlUI* pParent=pControl;
	CControlUI* pForm=pControl;
	while(pParent=pParent->GetParent())
		pForm=pParent;
	if(pForm!=m_LayoutManager.GetForm())
		return;

	m_MultiTracker.RemoveAll();
	if(pControl->IsVisible())
		m_MultiTracker.Add(CreateTracker(pControl));
	g_pPropertiesWnd->ShowProperty(pControl);

	this->Invalidate(FALSE);
}

void CUIDesignerView::OnActivated()
{
	g_pPropertiesWnd->ShowProperty(m_MultiTracker.GetFocused());
}

void CUIDesignerView::InitUI(CControlUI* pControl)
{
	if(pControl==NULL)
		return;

	ExtendedAttributes* pExtended=new ExtendedAttributes;
	ZeroMemory(pExtended,sizeof(ExtendedAttributes));
	pControl->SetTag((UINT_PTR)pExtended);

	pExtended->nClass=g_GetUIClass(pControl);
	g_pClassView->InsertUITreeItem(pControl);

	CContainerUI* pContainer=static_cast<CContainerUI*>(pControl->GetInterface(_T("Container")));
	if(pContainer==NULL)
		return;
	for (int i=0;i<pContainer->GetCount();i++)
	{
		InitUI(pContainer->GetItemAt(i));
	}
}

void CUIDesignerView::OnFormEditTest()
{
	m_LayoutManager.TestForm();
}

void CUIDesignerView::OnAlignLeft()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignLeft(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignRight()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignRight(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignTop()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignTop(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignBottom()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignBottom(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignCenterVertically()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignCenterVertically(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignCenterHorizontally()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignCenterHorizontally(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignHorizontal()
{

}

void CUIDesignerView::OnAlignVertical()
{

}

void CUIDesignerView::OnAlignSameWidth()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignSameWidth(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignSameHeight()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignSameHeight(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnAlignSameSize()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.AlignSameSize(m_MultiTracker.GetFocused(),arrSelected);
}

void CUIDesignerView::OnShowGrid()
{
	m_LayoutManager.ShowGrid(!m_LayoutManager.IsShowGrid());
}

void CUIDesignerView::OnShowAuxBorder()
{
	m_LayoutManager.ShowAuxBorder(!m_LayoutManager.IsShowAuxBorder());

	this->Invalidate(FALSE);
}

void CUIDesignerView::OnUpdateFormEditTest(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_LayoutManager.IsEmptyForm());
}

void CUIDesignerView::OnUpdateAlignLeft(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MultiTracker.GetSize()>1?TRUE:FALSE);
}

void CUIDesignerView::OnUpdateAlignRight(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MultiTracker.GetSize()>1?TRUE:FALSE);
}

void CUIDesignerView::OnUpdateAlignTop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MultiTracker.GetSize()>1?TRUE:FALSE);
}

void CUIDesignerView::OnUpdateAlignBottom(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MultiTracker.GetSize()>1?TRUE:FALSE);
}

void CUIDesignerView::OnUpdateAlignCenterVertically(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_MultiTracker.IsEmpty());
}

void CUIDesignerView::OnUpdateAlignCenterHorizontally(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_MultiTracker.IsEmpty());
}

void CUIDesignerView::OnUpdateAlignHorizontal(CCmdUI* pCmdUI)
{

}

void CUIDesignerView::OnUpdateAlignVertical(CCmdUI* pCmdUI)
{

}

void CUIDesignerView::OnUpdateAlignSameWidth(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MultiTracker.GetSize()>1?TRUE:FALSE);
}

void CUIDesignerView::OnUpdateAlignSameHeight(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MultiTracker.GetSize()>1?TRUE:FALSE);
}

void CUIDesignerView::OnUpdateAlignSameSize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_MultiTracker.GetSize()>1?TRUE:FALSE);
}

void CUIDesignerView::OnUpdateShowGrid(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_LayoutManager.IsShowGrid());
}

void CUIDesignerView::OnUpdateShowAuxBorder(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_LayoutManager.IsShowAuxBorder());
}

void CUIDesignerView::OnDestroy()
{
	__super::OnDestroy();

	// TODO: �ڴ˴�������Ϣ�����������
	g_pClassView->RemoveUITreeItem(m_LayoutManager.GetForm());
	g_pPropertiesWnd->HideAllProperties(TRUE,TRUE);
}

CPaintManagerUI* CUIDesignerView::GetManager() const
{
	return m_LayoutManager.GetForm()->GetManager();
}

void CUIDesignerView::OnEditCut()
{

}

void CUIDesignerView::OnEditCopy()
{

}

void CUIDesignerView::OnEditPaste()
{

}

void CUIDesignerView::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_MultiTracker.IsEmpty());
}

void CUIDesignerView::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	BOOL bOn=FALSE;
	if(CountClipboardFormats()!=0&&IsClipboardFormatAvailable(11))
		bOn=TRUE;

	pCmdUI->Enable(bOn);
}

void CUIDesignerView::OnMicoMoveUp()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.MicoMoveUp(arrSelected,MICO_MOVE_SPACE);
}

void CUIDesignerView::OnMicoMoveDown()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.MicoMoveDown(arrSelected,MICO_MOVE_SPACE);
}

void CUIDesignerView::OnMicoMoveLeft()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.MicoMoveLeft(arrSelected,MICO_MOVE_SPACE);
}

void CUIDesignerView::OnMicoMoveRight()
{
	CArray<CControlUI*,CControlUI*> arrSelected;

	m_MultiTracker.GetSelected(arrSelected);
	m_LayoutManager.MicoMoveRight(arrSelected,MICO_MOVE_SPACE);
}

void CUIDesignerView::OnShowPropertyDialog()
{
	if(m_MultiTracker.GetSize()>1)
		return;

	ShowPropertyDialog(m_MultiTracker.GetFocused());
}

void CUIDesignerView::ShowPropertyDialog(CControlUI* pControl)
{
	if(pControl==NULL)
		return;

	CDialog* pPropDlg=NULL;
	ExtendedAttributes* pExtended=(ExtendedAttributes*)pControl->GetTag();
	switch(pExtended->nClass)
	{
	case classForm:
		break;
	case classControl:
		break;
	case classLabel:
	case classText:
		break;
	case classButton:
		break;
	case classEdit:
		break;
	case classOption:
		break;
	case classProgress:
		break;
	case classSlider:
		break;
	case classCombo: 
		break;
	case classActiveX:
		break;
	case classContainer:
		break;
	case classVerticalLayout:
		break;
	case classDialogLayout:
		break;
	case classTabLayout:
		pPropDlg=new CPropertyTabLayoutUI(pControl);
		break;
	case classHorizontalLayout:
		break;
	case classTileLayout:
		break;
	default:
		return;
	}

	if(pPropDlg)
	{
		pPropDlg->DoModal();
		delete pPropDlg;
	}
}

void CUIDesignerView::ReleaseExtendedAttributes(CControlUI* pControl)
{
	if(pControl==NULL)
		return;

	ExtendedAttributes* pExtended=(ExtendedAttributes*)pControl->GetTag();
	delete pExtended;
	pControl->SetTag(NULL);

	CContainerUI* pContainer=static_cast<CContainerUI*>(pControl->GetInterface(_T("Container")));
	if(pContainer==NULL)
		return;
	for (int i=0;i<pContainer->GetCount();i++)
	{
		ReleaseExtendedAttributes(pContainer->GetItemAt(i));
	}
}