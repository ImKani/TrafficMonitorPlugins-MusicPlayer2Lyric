#include "pch.h"
#include "OptionsDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(COptionsDlg, CDialog)

COptionsDlg::COptionsDlg(CWnd* pParent)
    : CDialog(IDD_OPTIONS_DIALOG, pParent)
{
}

COptionsDlg::~COptionsDlg()
{
}

void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
END_MESSAGE_MAP()

BOOL COptionsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    SetDlgItemInt(IDC_MAX_DISPLAY_LINES_EDIT, m_data.max_display_lines);
    CheckDlgButton(IDC_FORCE_THREE_LINES_CHECK, m_data.force_three_lines);
    SetDlgItemInt(IDC_PRIMARY_FONT_PERCENT_EDIT, m_data.primary_font_percent);
    SetDlgItemInt(IDC_SUPPLEMENT_FONT_PERCENT_EDIT, m_data.supplement_font_percent);
    SetDlgItemInt(IDC_MIN_ITEM_WIDTH_EDIT, m_data.min_item_width);
    SetDlgItemInt(IDC_MAX_ITEM_WIDTH_EDIT, m_data.max_item_width);

    return TRUE;
}

void COptionsDlg::OnOK()
{
    m_data.max_display_lines = GetDlgInt(IDC_MAX_DISPLAY_LINES_EDIT, m_data.max_display_lines, 1, 3);
    m_data.force_three_lines = (IsDlgButtonChecked(IDC_FORCE_THREE_LINES_CHECK) != 0);
    m_data.primary_font_percent = GetDlgInt(IDC_PRIMARY_FONT_PERCENT_EDIT, m_data.primary_font_percent, 50, 100);
    m_data.supplement_font_percent = GetDlgInt(IDC_SUPPLEMENT_FONT_PERCENT_EDIT, m_data.supplement_font_percent, 50, 100);
    m_data.min_item_width = GetDlgInt(IDC_MIN_ITEM_WIDTH_EDIT, m_data.min_item_width, 40, 1000);
    m_data.max_item_width = GetDlgInt(IDC_MAX_ITEM_WIDTH_EDIT, m_data.max_item_width, m_data.min_item_width, 2000);

    CDialog::OnOK();
}

int COptionsDlg::GetDlgInt(UINT id, int default_value, int min_value, int max_value) const
{
    BOOL translated{ FALSE };
    int value{ static_cast<int>(GetDlgItemInt(id, &translated, FALSE)) };
    if (!translated)
        value = default_value;
    return min(max(value, min_value), max_value);
}
