// AttentionView.cpp : implementation of the CAttentionView class
// Project MOT
// Kazanovich June 2005

#include "stdafx.h"
#include "Attention.h"

#include "AttentionDoc.h"
#include "AttentionView.h" 

#ifdef _DEBUG
#define new DEBUG_NEW 
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAttentionView

IMPLEMENT_DYNCREATE(CAttentionView, CScrollView)

BEGIN_MESSAGE_MAP(CAttentionView, CScrollView)
	//{{AFX_MSG_MAP(CAttentionView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttentionView construction/destruction

CAttentionView::CAttentionView()
{
	// TODO: add construction code here

}

CAttentionView::~CAttentionView()
{
}

BOOL CAttentionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAttentionView drawing

#define RED	RGB(150, 0, 0)
#define BLUE RGB(0, 0, 200)
#define GREEN RGB(0, 150, 0)
#define LIGHT_RED RGB(255, 50, 50)
#define LIGHT_GREEN RGB(50, 255, 50)
#define BLACK RGB(0, 0, 0)
#define CYAN RGB(30, 255, 255)

#include "mot.h"

void Connec();
void My_error(CString);
void Netalloc();
void Netinit();
void Netreinit();
void Pardef();
void Step();
void Trajclose();
void Trajopen();
void Trajsave();
void Stimgen();
void Stimvar(double t);

extern struct network net[noaf];
extern struct parameters par;
extern struct integration integr;
extern struct image im;

const float scalex = 1;
const float scaley = 7;

const int sqx = 2;	// Scale (not less than 2)
const int sqy = 2;
const int sqx0 = 10;// Origine of the view
const int sqy0 = 10;
const int pmax = 7;	// (pmax + 1) maximal number of frames in the row

int it;	// the number of iterations

void CAttentionView::OnDraw(CDC* pDC)
{
	int i;
	COLORREF color, palett[7] = {RED, BLUE, GREEN, LIGHT_RED, LIGHT_GREEN, CYAN, BLACK};
	
	Pardef();
	int n = par.n;
	double resthresh = integr.resthresh; 
	double maxt = integr.maxt;
	double dt = integr.dt;

	it = 0;
	double t = 0;
	int p = 0, q = 0;	// coordinates of frames (viewports)

    #ifdef RANDOMIZE_ON
	srand(time(NULL));
	#endif

	Stimgen();
	#ifdef TRAJSAVE_ON
		Trajopen();
	#endif
	Netalloc();
	Netinit();
	Connec();

	while (t <= maxt)
	{//1+
		// Displaying the current state
		if (it%10 == 0)
		{//2+
			// Viewport formation
			CPoint view_origine(sqx0, sqy0);;
			CPoint viewport_origine( (ncolumns + 1)*p*sqx,
								(nrows + 1)*q*sqy);
			viewport_origine += view_origine;
			pDC->SetViewportOrg(viewport_origine);
			int viewport_sizex = ncolumns*sqx;
			int viewport_sizey = nrows*sqy;
			CSize viewport_size(viewport_sizex, viewport_sizey);
			pDC->SetViewportExt(viewport_size);
		
			// Frame drawing
			pDC->SelectStockObject(BLACK_PEN);
			pDC->SelectStockObject(WHITE_BRUSH);
			pDC->Rectangle(0, 0, viewport_size.cx, viewport_size.cy);

			for (i = 1; i <= n; i++)
			{//3+
				if (net[0].osc[i].amp > 0)
				{//4+
					int i1 = i - 1;			// enumaration starts from 1
					int col = i1%ncolumns;	// column in the frame
					int row = i1/ncolumns;	// row in the frame
					int x = col*sqx;			// position in the frame
					int y = row*sqy;
					int x1 = x;
					int y1 = y;
					int x2 = x1 + sqx; 
					int y2 = y1 + sqy;
				
					color = palett[6]; 
					for (int oaf = 0; oaf < noaf; oaf++)
					{
						if (net[oaf].osc[i].amp > resthresh)
						{	color |= palett[oaf]; }
					}
					CPen penSolid(PS_SOLID, 1, color);
					CBrush brushFilled(color);
					pDC->SelectObject(penSolid);
					pDC->SelectObject(brushFilled);
					pDC->Rectangle(x1, y1, x2, y2);
				}//4-
			}//3-

			// Marking targets
			for (i = 0; i < nobj; i++)
			{
				if (im.obj[i].type == target)
				{
					int x1 = (im.obj[i].cx - im.r - 1)*sqx;
					int y1 = (im.obj[i].cy - im.r)*sqx;
					int x2 = (im.obj[i].cx + im.r + 1)*sqx;
					int y2 = (im.obj[i].cy + im.r + 2)*sqx;
					CPen penSolid(PS_SOLID, 2, palett[5]);
					pDC->SelectObject(penSolid);
					pDC->SelectStockObject(HOLLOW_BRUSH);
					pDC->Rectangle(x1, y1, x2, y2);
				}
			}

			// Indexing output frame locations
			p++;
			if (p > pmax)
			{
				p = 0;
				q++;
			}

		}//2-

		#ifdef TRAJSAVE_ON
			Trajsave();
		#endif
		Step();

		if ( (t > integr.expos) && (t < integr.maxt - integr.probe) && 
			(it%integr.movement_dt == 0) ) 
		{	
			Stimvar(t);
			Netreinit(); 
			Connec();
		}

		t += dt;
		it++;
	}//1-
	#ifdef TRAJSAVE_ON
		Trajclose();
	#endif
}

void CAttentionView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 3000;
	sizeTotal.cy = 3000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CAttentionView printing

BOOL CAttentionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAttentionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAttentionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAttentionView diagnostics

#ifdef _DEBUG
void CAttentionView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAttentionView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CAttentionDoc* CAttentionView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAttentionDoc)));
	return (CAttentionDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAttentionView message handlers
