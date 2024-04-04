#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>

class BasicDrawPane : public wxPanel
{
public:
    BasicDrawPane(wxFrame* parent);
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void render(wxDC& dc);
    wxDC *dc2;
    int liczba, selectedFunction, angle;
    int min_x, max_x, center_x, center_y;
    DECLARE_EVENT_TABLE()
};

class MyApp: public wxApp
{
    bool OnInit();
    void onScroll(wxScrollEvent& event); //przesuwanie slidera
    void onSave(wxCommandEvent& event); //do przycisku zapisywania 
    void onEditXmin(wxCommandEvent& event); // zmiana x min
    void onEditXmax(wxCommandEvent& event); 
    void onEditCenterX(wxCommandEvent& event); 
    void onEditCenterY(wxCommandEvent& event); 
    void onListBoxSelect(wxCommandEvent& event); // zaznaczenie rodzaju wykresu 
    void onScrollAngle(wxScrollEvent& event); 
    wxFrame *frame;
    BasicDrawPane * drawPane;
    wxSlider *slider;
    wxSlider *sliderAngle;
    wxButton *button;
    wxTextCtrl *textctrl1;
    wxTextCtrl *textctrl2;
    wxTextCtrl *textctrl3;
    wxTextCtrl *textctrl4;
    wxListBox *listBox;
public:
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Hello wxDC"), wxPoint(50,50), wxSize(800,600));
    drawPane = new BasicDrawPane( (wxFrame*) frame);
    drawPane->liczba = 100;
    drawPane->selectedFunction = 1;
    drawPane->center_x = 200;
    drawPane->center_y = 200;
    drawPane->angle = 0;
    sizer->Add(drawPane, 1, wxEXPAND);
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
    frame->Show();
    int ID_BUTTON = 1;
    int ID_SLIDER = 100;
    int ID_SLIDER_ANGLE = 110;
    int ID_TEXTCTRL1 = 101;
    int ID_TEXTCTRL2 = 102;
    int ID_TEXTCTRL3 = 105;
    int ID_TEXTCTRL4 = 106;
    this->drawPane->min_x = -50;
    this->drawPane->max_x = 50;
    this->button = new wxButton(drawPane, ID_BUTTON, wxT("Save"), 
                        wxPoint(600, 20));
    this->textctrl1 = new wxTextCtrl(drawPane, ID_TEXTCTRL1, wxT("-50"), 
                        wxPoint(650, 60), wxSize(50, -1)); //wpisywanie wartosci x 
    this->textctrl2 = new wxTextCtrl(drawPane, ID_TEXTCTRL2, wxT("50"), 
                        wxPoint(650, 90), wxSize(50, -1));
    this->listBox = new wxListBox(drawPane, 103, 
                            wxPoint(600, 140), wxSize(160, -1), 0, NULL, 0);
    this->slider = new wxSlider(drawPane, ID_SLIDER, 100, 0, 200, 
                        wxPoint(600, 310), wxSize(140, -1));
    this->textctrl3 = new wxTextCtrl(drawPane, ID_TEXTCTRL3, wxT("200"), //do wpisywania wsp srodka wykrresu
                        wxPoint(650, 390), wxSize(50, -1)); 
    this->textctrl4 = new wxTextCtrl(drawPane, ID_TEXTCTRL4, wxT("200"), 
                        wxPoint(650, 450), wxSize(50, -1));  

    this->sliderAngle = new wxSlider(drawPane, ID_SLIDER_ANGLE, 0, 0, 360, 
                        wxPoint(600, 510), wxSize(140, -1));

    this->listBox->Append( wxT("y = x + sin(4x)") );
    this->listBox->Append( wxT("y = x^2") );
    this->listBox->Append( wxT("y = 0.5 * e ^ (4x - 3x^2)") );
    this->listBox->SetSelection(drawPane->selectedFunction, true);
    Connect(ID_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, 
        wxCommandEventHandler(MyApp::onSave));
    Connect(ID_SLIDER, wxEVT_COMMAND_SLIDER_UPDATED, 
        wxScrollEventHandler(MyApp::onScroll)); 
    Connect(ID_TEXTCTRL1, wxEVT_COMMAND_TEXT_UPDATED, 
        wxCommandEventHandler(MyApp::onEditXmin));   
    Connect(ID_TEXTCTRL2, wxEVT_COMMAND_TEXT_UPDATED, 
        wxCommandEventHandler(MyApp::onEditXmax)); 
    Connect(103, wxEVT_COMMAND_LISTBOX_SELECTED, 
        wxCommandEventHandler(MyApp::onListBoxSelect)); 
    Connect(ID_TEXTCTRL3, wxEVT_COMMAND_TEXT_UPDATED, 
        wxCommandEventHandler(MyApp::onEditCenterX));   
    Connect(ID_TEXTCTRL4, wxEVT_COMMAND_TEXT_UPDATED, 
        wxCommandEventHandler(MyApp::onEditCenterY)); 
    Connect(ID_SLIDER_ANGLE, wxEVT_COMMAND_SLIDER_UPDATED, 
        wxScrollEventHandler(MyApp::onScrollAngle)); 
    return true;
} 

void MyApp::onSave(wxCommandEvent& event)
{
    wxInitAllImageHandlers();
    std::cout << "save event " << std::endl; //wypisanie w terminalu
    event.Skip();
    wxBitmap myBitmap(200, 200); 
    wxMemoryDC dc;
    dc.SelectObject(myBitmap);
    wxFont font=dc.GetFont();
    font.SetPointSize(15);
    dc.SetTextForeground(*wxBLACK);
    dc.DrawRectangle(0,0,50,100);
    wxString s(_T("A"));
    dc.DrawText(s, 5,5);
    //setting propper file path
    wxFileName f(wxStandardPaths::Get().GetExecutablePath()); //pobieranie sciezki do katalogu
    wxString appPath(f.GetPath());
    wxString path(appPath + _T("/test.png"));
    myBitmap.SaveFile(path, wxBITMAP_TYPE_PNG);
}

void MyApp::onEditXmin(wxCommandEvent& event)
{
    // wxAtoi(str) - convert wxString to integer number
    this->drawPane->min_x = wxAtoi(this->textctrl1->GetValue()); 
    this->drawPane->paintNow();
}

void MyApp::onEditXmax(wxCommandEvent& event)
{
    this->drawPane->max_x = wxAtoi(this->textctrl2->GetValue());
    this->drawPane->paintNow();
}

void MyApp::onEditCenterX(wxCommandEvent& event)
{
    this->drawPane->center_x = wxAtoi(this->textctrl3->GetValue()); 
    this->drawPane->paintNow();
}

void MyApp::onEditCenterY(wxCommandEvent& event)
{
    this->drawPane->center_y = wxAtoi(this->textctrl4->GetValue());
    this->drawPane->paintNow();
}

void MyApp::onListBoxSelect(wxCommandEvent& event)
{
    this->drawPane->selectedFunction = this->listBox->GetSelection();
    this->drawPane->paintNow();
}

void MyApp::onScrollAngle(wxScrollEvent& event)
{
    this->drawPane->angle = sliderAngle->GetValue();
    this->drawPane->paintNow();
}
BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
EVT_PAINT(BasicDrawPane::paintEvent)
END_EVENT_TABLE()
BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
}

void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
    this->dc2 = &dc;
}

void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

//y = x + sin(4x)
int calculate0(int x)
{
    return x + 4 * (int)sin(x);
}

//y = x^2
int calculate1(int x)
{
    // return x*x;
    return pow(x, 2);
}

//y = 0.5 * e ^ (4x - 3x^2)"
int calculate2(int x)
{
    int wykladnik = 4 * x - 3 * pow(x, 2);
    return 0.5 * pow(M_E, wykladnik);
}

void calculateAngleTransformation(wxPoint points[],int size, int angle, int centerx, int centery)
{
    std::cout << "angle " << angle << std::endl; 
    
    //converting angle to radians for sin and cos
    float radians = angle * (M_PI / 180);

    std::cout << "radians " << radians << std::endl; 

    for (int i = 0; i < size-1; i++) {
        int x = points[i].x - centerx;
        int y = points[i].y - centery;
        int x_new = x * cos(radians) - y * sin(radians);
        int y_new = x * sin(radians) + y * cos(radians);
        points[i].x = x_new + centerx;
        points[i].y = y_new + centery;   
    }
}
void BasicDrawPane::render(wxDC&  dc)
{
    dc.DrawRectangle(0,0,580,500);
    dc.DrawText(wxString::Format("min x"), 600, 60);
    dc.DrawText(wxString::Format("max x"), 600, 90);
    dc.DrawText(wxString::Format("wspolrzedna x srodka wykresu"), 600, 360);
    dc.DrawText(wxString::Format("wspolrzedna y srodka wykresu"), 600, 420);
    int centerx = center_x; //wspolrzedne srodka wykresu
    int centery = center_y;
    dc.DrawLine(centerx, centery-200, centerx, centery+200);
    dc.DrawLine(centerx-200, centery, centerx+200, centery);
    wxPoint *points = new wxPoint[liczba];

    for (int i = 0; i < liczba; i++) {
        int x = min_x + i + centerx;
        int y = 0;

        switch(selectedFunction) {
        case 0:
            y = calculate0(min_x + i);
            break;
        case 1:
            y = calculate1(min_x + i);
            break;
        default:
            y = calculate2(min_x + i);
        }

        y = (-1) * y + centery;
        if (min_x + i <= max_x) {
            points[i].x = x;
            points[i].y = y;
        } else {
            points[i].x = points[i-1].x;
            points[i].y = points[i-1].y;
        }
    }
    if (angle > 0) {
        calculateAngleTransformation(points, liczba, angle, centerx, centery);
    }

    for (int i = 0; i < liczba-1; i++) {
        //dc.DrawLine(points[i].x, points[i].y , points[i+1].x, points[i+1].y);
        dc.DrawPoint(points[i].x, points[i].y);
    }
    dc.DrawText(wxString::Format("Liczba rysowanych punktow:"), 600, 260);    
    dc.DrawRotatedText(wxString::Format(wxT("%i"), liczba), 600, 280, 0);
}

void MyApp::onScroll(wxScrollEvent& event)
{
    int slider_val = slider->GetValue();
    this->drawPane->liczba = slider_val;
    this->drawPane->paintNow();
}

