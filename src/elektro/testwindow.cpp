/*
 *  elektrOGame
 */

#include "testwindow.h"

#include <sstream>
#include <istream>
#include <vector>
#include <math.h>
#include <guichan/widgets/label.hpp>

#include "gui/widgets/button.h"
#include "gui/widgets/chattab.h"
#include "gui/widgets/scrollarea.h"
#include "gui/widgets/listbox.h"
#include "gui/widgets/textbox.h"
#include "gui/widgets/radiobutton.h"
#include "log.h"

#include "elektro/lang_tr.h"
#include "../resources/resourcemanager.h"
#include "../graphics.h"
#include "../particle.h"
#include "../sound.h"
#include "../npc.h"
#include "net/ea/npchandler.h"
#include "utils/stringutils.h"
#include "../localplayer.h"
#include "elektro/typedef.h"
#include "../beingmanager.h"
#include "utils/stringutils.h"
#include "utils/gettext.h"

extern std::stringstream mQuestion;
extern int sira;
extern int sure;
int TestDialog::gecenZaman=0;
const int padX = 50;
const int padY = 50;

gcn::Label *mTimeLabel = new gcn::Label("time label");
TestDialog::TestDialog():
    Window(_("Soran Üstat"))
{
    setContentSize(350, 275);
    setSize(350,350);
    setPosition(100,100);

    cancelButton = new Button(BTNCANCEL, "cancel", this);
    mEvaluate = new Button(toTurkish("Değerlendir!"),"ok",this);
    mFinishClose = new Button("Pencereyi kapat","startcancel",this);

    ResourceManager *resman = ResourceManager::getInstance();
    testinfo = resman->getImage("graphics/elektrik/testtoolbar.png");
    mStartBox = new BrowserBox();
    mStartBox->setOpaque(false);

    mStartScroll = new ScrollArea(mStartBox);
    mStartScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mStartScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mStartScroll->setVisible(false);
    mStartScroll->setWidth(250);
    mStartScroll->setHeight(200);
    mStartScroll->setY(50);


    mFinishText = new BrowserBox();
    mFinishScroll = new ScrollArea(mFinishText);
    mFinishScroll->setWidth(250);
    mFinishScroll->setHeight(285);
    mFinishScroll->setY(30);
    mFinishScroll->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_NEVER);
    mFinishScroll->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
    mFinishScroll->setVisible(false);
    add(mFinishScroll);

    mMessageText = new BrowserBox();
    mMessageArea = new ScrollArea(mMessageText);
    mMessageArea->setWidth(getWidth()-90);
    mMessageArea->setHeight(getHeight()-70);
    mMessageArea->setPosition(40,25);
    mMessageArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
    mMessageArea->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_AUTO);
    mMessageArea->setVisible(false);
    add(mMessageArea);
    mMessageOk = new Button("Tamam","mesajok",this);
    mMessageOk->setVisible(false);
    mMessageOk->setPosition((getWidth()-mMessageOk->getWidth())/2,
                           getHeight()-mMessageOk->getHeight()-10);
    add(mMessageOk);

    mStartOk = new Button("Hemen Başla","startok",this);
    mStartCancel = new Button("Yok Almayım","startcancel",this);

    mStartCancel->setWidth(mStartOk->getWidth());
    mStartCancel->setHeight(mStartOk->getHeight());

    lblCurrentQuestion = new gcn::Label("Soru : 1");
    lblCurrentQuestion->setPosition(130,25);
    lblCurrentQuestion->setFont(boldFont);
    lblCurrentQuestion->setForegroundColor(gcn::Color(50,250,250));

    int x=108, y=52;

    lblTotalQuestion = new gcn::Label("2");
    lblTotalQuestion->setPosition(x,y);
    lblTotalQuestion->setFont(boldFont);
    lblTotalQuestion->setForegroundColor(gcn::Color(50,50,150));
    x+=88;

    lblAward = new gcn::Label("3");
    lblAward->setFont(boldFont);
    lblAward->setForegroundColor(gcn::Color(0,200,20));
    lblAward->setPosition(x,y);
    x+=88;

    lblPunish = new gcn::Label("4");
    lblPunish->setFont(boldFont);
    lblPunish->setForegroundColor(gcn::Color(250,120,120));
    lblPunish->setPosition(x,y);
    x=108;
    y=75;

    lblTotalTime = new gcn::Label("5");
    lblTotalTime->setPosition(x,y);
    lblTotalTime->setFont(boldFont);
    lblTotalTime->setForegroundColor(gcn::Color(0, 0, 200));
    x+=88;

    lblRemainingTime = new gcn::Label("6");
    lblRemainingTime->setPosition(x,y);
    lblRemainingTime->setFont(boldFont);
    lblRemainingTime->setForegroundColor(gcn::Color(200, 20, 20));
    x+=88;

    lblSuccessLimit = new gcn::Label("7");
    lblSuccessLimit->setPosition(x , y);
    lblSuccessLimit->setFont(boldFont);
    lblSuccessLimit->setForegroundColor(gcn::Color(0, 200, 200));
    x+=88;

//    iPerReminingQTime   =new gcn::Label("8");
//    iPerReminingQTime->setPosition(x,y);
//    iPerReminingQTime->setFont(hitBlueFont);
//    iPerReminingQTime->setForegroundColor(gcn::Color(0,0,200));

    lblCurrentQuestion->adjustSize();
    lblTotalQuestion->adjustSize();
    lblAward->adjustSize();
    lblPunish->adjustSize();
    lblTotalTime->adjustSize();
    lblRemainingTime->adjustSize();
    lblSuccessLimit->adjustSize();
//    iPerReminingQTime->adjustSize();

    add(lblTotalTime);
    add(lblPunish);
    add(lblAward);
    add(lblRemainingTime);
    add(lblTotalQuestion);
    add(lblCurrentQuestion);
    add(lblSuccessLimit);
//    add(iPerReminingQTime);

    cancelButton->setPosition(
            260 - 5 - cancelButton->getWidth(),
            275 - 5 - cancelButton->getHeight());

    cancelButton->setVisible(false);
    mFinishClose->setVisible(false);

    mEvaluate->setPosition(50,getHeight()-50);
    mEvaluate->setVisible(false);

    mBasla = false;
    mMesajMod = false;

    add(cancelButton);
    add(mStartOk);
    add(mStartCancel);
    add(mStartScroll);
    add(mEvaluate);
    add(mFinishClose);

    setLocationRelativeTo(getParent());
}

int
TestDialog::getNumberOfElements()
{
    return 0 ;//mItems.size();
}

void
TestDialog::setupPositionInfoLabels()
{
    int x, y;
    x = getWidth()/2.0 - lblCurrentQuestion->getWidth()/2.0;
    y = 23;
    lblCurrentQuestion->setPosition(x,y);

    x = getWidth()/2.0 - 79;
    y = 53;
    lblTotalQuestion->setPosition(x,y);

    x = getWidth()/2.0 + 12;
    lblAward->setPosition(x,y);

    x = getWidth()/2.0 + 102;
    lblPunish->setPosition(x,y);

    y = 77;
    x = getWidth()/2.0 - 79;
    lblSuccessLimit->setPosition(x , y);

    x = getWidth()/2.0 + 12;
    lblTotalTime->setPosition(x,y);

    x = getWidth()/2.0 + 102;
    lblRemainingTime->setPosition(x,y);

}

void
TestDialog::logic()
{
    if (isVisible() == false) return;
    static int m=0;
    if (mBasla) m++;
    if (m==100)
    {
        m=0;
        gecenZaman++;
        lblRemainingTime->setCaption(toString(mTotalTime-gecenZaman));
        lblRemainingTime->adjustSize();
        if (mTotalTime==gecenZaman)
        {
            action(gcn::ActionEvent(NULL, "ok"));
        }
    }
//    setupPositionInfoLabels();
    Window::logic();
}

void
TestDialog::showQuestion()
{
      for (miImage = mvImage.begin();
            miImage != mvImage.end();
            miImage++)
      {
          miImage->visible = true;
      }

      for (miAnim = mvAnim.begin();
            miAnim != mvAnim.end();
            ++miLabel)
      {
          miAnim->visible=true;
      }

      for (miRadio = mvRadio.begin();
            miRadio!=mvRadio.end();
            ++miRadio)
      {
          miRadio->radio->setVisible(true);
      }

      for (miCheck = mvCheck.begin();
            miCheck != mvCheck.end();
            ++miCheck)
      {
          miCheck->check->setVisible(true);
      }
      for (miLabel = mvLabel.begin();
            miLabel != mvLabel.end();
            ++miLabel)
      {
          (*miLabel)->setVisible(true);
      }
}

void
TestDialog::start()
{
    int messageWinX;
    int messageWinY;

    switch(testState)
    {
        case MESSAGE_STATE:
            hideEverything();
            showMessage();
            setContentSize(350, 350);
            messageWinX = (1024 - 350) / 2;
            messageWinY = (768 - 350) / 2;
            setPosition(messageWinX , messageWinY);
            break;
        case ONE_RADIO_STATE:
        case ONE_CEHCK_STATE:
            gecenZaman=0;
            mBasla = true;
            hideStart();
            mEvaluate->setVisible(true);
            cancelButton->setVisible(false);
            showInfoLabels();
            showQuestion();
            break;
        case ONEQ_START_STATE:
            gecenZaman=0;
            mBasla=true;
            action(gcn::ActionEvent(NULL, "startok"));
            break;
        case MANYQ_START_SATATE:
            hideInfoLabels();
            gecenZaman=0;
            mBasla = false;
            mStartBox->clearRows();
            mStartBox->addRow("##3==========================================");
            mStartBox->addRow("               ##1Teste Hoşgeldin!");
            mStartBox->addRow("##3==========================================");
            mStartBox->addRow("");
            mStartBox->addRow("  ##2Soru Sayısı     :##6"+toString(mTotalQuestion));
            mStartBox->addRow("  ##2Toplam Süre     :##6"+toString(mTotalTime)+"##2 saniye");
            mStartBox->addRow("  ##2Başarı Sınırı   :##6"+toString(mSuccessLimit)+"##2 soru");
            mStartBox->addRow("  ##2Soru Başına Süre:##6"+toString(mTotalTime/mTotalQuestion)+"##2 saniye");
            mStartBox->addRow("  ##2Ödül            :##6"+toString(mAward));
            mStartBox->addRow("  ##2Ceza            :##6"+toString(mPunish));
            mStartBox->addRow("##7=========================================");
            mStartBox->addRow("##8      ============================       ");
            mStartBox->addRow("##9           ==================            ");

            mStartScroll->setX((getWidth()-mStartScroll->getWidth())/2);
            mStartScroll->setVisible(true);

            mStartOk->setPosition(getWidth()/2-mStartOk->getWidth()-5,
                                  mStartScroll->getY()+mStartScroll->getHeight()+20);
            mStartCancel->setPosition(getWidth()/2+5,mStartOk->getY());
            mStartCancel->setVisible(true);
            mStartOk->setVisible(true);
            mEvaluate->setVisible(false);
            putPlaceButtons();
            break;
        case MANYQ_CONTINUE_STATE:
            showInfoLabels();
            showQuestion();
            for(miButton = mvButton.begin();miButton !=mvButton.end();++miButton)
            {
                (*miButton)->setVisible(true);
            }
            break;
        case TEST_FINISHED_STATE:
            gecenZaman=0;
            mFinishScroll->setVisible(true);

            mFinishText->setWidth(getWidth()-40);
            mFinishText->setHeight(getHeight()-150);
            mFinishText->setVisible(true);
            mFinishClose->setVisible(true);
            mFinishClose->setPosition((getWidth()-mFinishClose->getWidth())/2,getHeight()-100);
            mEvaluate->setVisible(false);
            setVisible(false);
            mBasla = false;
            reset();
            current_npc=0;
            NPC::isTalking = false;
            break;
        case SHOW_RESULT_STATE:
            reset();
            deleteQuestionButtons();
            showFinishMessage();
            mBasla = false;
            break;
    }

//    hideMessage();
    lblAward->setCaption(toString(mAward));
    lblPunish->setCaption(toString(mPunish));
    lblTotalTime->setCaption(toString(mTotalTime));
    lblRemainingTime->setCaption(toString(mTotalTime-gecenZaman));
    lblTotalQuestion->setCaption(toString(mTotalQuestion));
    lblCurrentQuestion->setCaption("Soru :"+toString(mQuestionNumber));
    lblSuccessLimit->setCaption(toString(mSuccessLimit));
//    iPerReminingQTime->setCaption(toString(mQuestionPerTime));

    lblCurrentQuestion->adjustSize();
    lblTotalQuestion->adjustSize();
    lblAward->adjustSize();
    lblPunish->adjustSize();
    lblTotalTime->adjustSize();
    lblRemainingTime->adjustSize();
    lblSuccessLimit->adjustSize();
//    iPerReminingQTime->adjustSize();
}

void
TestDialog::putPlaceButtons()
{
    int prevnextHeight;
    int butHeight = 20;
    int butWidth  = 20;
    int butGap    = 3;
    int firstX;
    int lastX;

    if (mTotalQuestion < 11)
    {
        prevnextHeight = butHeight;
        firstX = getWidth() / 2 - (mTotalQuestion /2.0) * (butWidth + butGap+3);
        lastX = getWidth() / 2 + (mTotalQuestion /2.0) * (butWidth + butGap)- butGap*2;
    }
    else
    {
        firstX = (getWidth() - testinfo->getWidth()) / 2.0 + 20;
        lastX = firstX + (butWidth + butGap) * 10 + butGap;
        prevnextHeight = (mTotalQuestion / 10 + 1) *
                         (butHeight + butGap) - butGap;
    }

    int x= firstX;
    int y= testinfo->getHeight() + 20;

    //prev button
    Button *but1 = new Button("<","go_prev",this);
    but1->setWidth(butWidth);
    but1->setHeight(prevnextHeight);
    but1->setPosition(x-24, y);
    but1->setVisible(false);
    add(but1);
    mvButton.push_back(but1);

    //next button
    Button *but2 = new Button(">","go_next",this);
    but2->setWidth(butWidth);
    but2->setHeight(prevnextHeight);
    but2->setPosition(lastX, y);
    but2->setVisible(false);
    add(but2);
    mvButton.push_back(but2);

    for (int i=1; i<mTotalQuestion+1; i++)
    {
        Button *but = new Button(toString(i),"goto_"+toString(i),this);
        but->setWidth(butWidth);
        but->setHeight(butHeight);
        but->setPosition(x, y);
        x += but->getWidth()+ 3;
        if (i%10==0)
        {
            if (mTotalQuestion<20)
                x = getWidth() / 2 - ((20-mTotalQuestion) /2.0) * (butWidth + butGap+4);
            else
                x = firstX;
            y += but->getHeight() + 3;
        }

        but->setVisible(false);
        add(but);
        mvButton.push_back(but);
    }
}

void
TestDialog::showFinishMessage()
{
    mEvaluate->setVisible(false);
    setWidth (400);
    setHeight(430);
    setPosition( (1024 - getWidth() ) /2 , (768 - getHeight()) / 2);
    mFinishScroll->setVisible(true);
    mFinishClose->setX((getWidth() - mFinishClose->getWidth())/2);
    mFinishClose->setY(getHeight()- mFinishClose->getHeight() - 80);
    mFinishClose->setVisible(true);
}

void
TestDialog::showInfoLabels()
{
    lblCurrentQuestion->setVisible(true);
    lblTotalQuestion->setVisible(true);
    lblAward->setVisible(true);
    lblPunish->setVisible(true);
    lblTotalTime->setVisible(true);
    lblRemainingTime->setVisible(true);
    lblSuccessLimit->setVisible(true);
}

void
TestDialog::hideInfoLabels()
{
    lblCurrentQuestion->setVisible(false);
    lblTotalQuestion->setVisible(false);
    lblAward->setVisible(false);
    lblPunish->setVisible(false);
    lblTotalTime->setVisible(false);
    lblRemainingTime->setVisible(false);
    lblSuccessLimit->setVisible(false);
}

void
TestDialog::hideEverything()
{
    hideMessage();
    hideStart();
    hideFinish();
    hideInfoLabels();
    mEvaluate->setVisible(false);
    cancelButton->setVisible(false);
}

void
TestDialog::hideMessage()
{
    mMessageArea->setVisible(false);
    mMessageOk->setVisible(false);
    mMessageText->setVisible(false);
}

void
TestDialog::showMessage()
{
    mMessageArea->setVisible(true);
    mMessageOk->setVisible(true);
    mMessageText->setVisible(true);
}

void
TestDialog::hideStart()
{
    mStartBox->setVisible(false);
    mStartCancel->setVisible(false);
    mStartOk->setVisible(false);
    mStartScroll->setVisible(false);
}

void
TestDialog::hideFinish()
{
    mFinishScroll->setVisible(false);
    mFinishText->setVisible(false);
    mFinishClose->setVisible(false);
}

void
TestDialog::deleteQuestionButtons()
{
    for(miButton = mvButton.begin();
        miButton != mvButton.end();
        ++miButton)
    {
        delete(*miButton); //eski buttonları sil
    }
    mvButton.clear();
}

void
TestDialog::draw(gcn::Graphics *graphics)
{
    Window::draw(graphics);
    Graphics *g = static_cast<Graphics*>(graphics);
    ResourceManager *resman = ResourceManager::getInstance();
    int xx = (getWidth()-testinfo->getWidth())/2;
    if (mBasla) g->drawImage(testinfo,xx,50);
    for (miImage = mvImage.begin();
            miImage != mvImage.end();
            ++miImage)
    {
         if (miImage->visible)
            g->drawImage(miImage->img,miImage->x,miImage->y);
    }

     for (miAnim = mvAnim.begin();
           miAnim != mvAnim.end();
           ++miAnim)
     {
        if (miAnim->visible)
        {
            Image *mImage1 = miAnim->anim->getCurrentImage();
            g->drawImage(mImage1,miAnim->x,miAnim->y);
            miAnim->anim->update(miAnim->v);
        }
     }
    drawChildren(graphics);
}

std::string
TestDialog::getElementAt(int i)
{
    return 0; //mItems[i];
}

void
TestDialog::parseItems(const std::string &itemString)
{

}

void
TestDialog::reset()
{
      for (miRadio = mvRadio.begin();
            miRadio != mvRadio.end();
            ++miRadio)
      {
          delete miRadio->radio;
      }

      for (miCheck = mvCheck.begin();
            miCheck != mvCheck.end();
            ++miCheck)
      {
          delete miCheck->check;
      }
      for (miLabel = mvLabel.begin();
            miLabel!=mvLabel.end();
            ++miLabel)
      {
          delete(*miLabel);
      }
      for (miTextBox = mvTextBox.begin();
            miTextBox != mvTextBox.end();++miTextBox)
      {
          delete(*miTextBox);
      }
      for (miScrollArea = mvScrollArea.begin();
            miScrollArea != mvScrollArea.end();++miScrollArea)
      {
          delete(*miScrollArea);
      }
      mvImage.clear();
      mvAnim.clear();
      mvRadio.clear();
      mvCheck.clear();
      mvLabel.clear();
      mvScrollArea.clear();
      mvTextBox.clear();
      mBitti = false;
      mMesajMod = false;
}

void
TestDialog::action(const gcn::ActionEvent &event)
{
    int choice = 0;
    std::string ch=event.getId();
//    chatWindow->chatLog(event.getId().substr(0,13), BY_SERVER);

    if (event.getId() == "ok")
    {
        // Send the selected index back to the server
        if (mTotalQuestion==1)
        {
            if(mType=="radio")
            {
                int answer=1;  //boş bırakılmışsa yanlış kabul et
                for (miRadio = mvRadio.begin();
                    miRadio != mvRadio.end();
                    ++miRadio)
                {
                    if (miRadio->radio->isSelected()) answer = miRadio->value +1 ;  //
                }
                Net::getNpcHandler()->listInput(current_npc, answer);
                testState = TEST_FINISHED_STATE;
            }
            else if(mType=="check")
            {
                testState = TEST_FINISHED_STATE;
                //yeni checkbox sisteminde gereksiz
//                int tru=0;  //boş bırakılmışsa yanlış kabul et
//                int sayac=0;
//                for (miCheck = mvCheck.begin();
//                        miCheck != mvCheck.end();
//                        ++miCheck)
//                {
//                sayac++;
//                    if ((miCheck->check->isMarked() && miCheck->value=="1") ||
//                        (!(miCheck->check->isMarked()) && miCheck->value=="2"))
//                        {
//                          tru++;
//                          chatWindow->chatLog("Secenek "+toString(sayac) +" dogru isaretlendi", BY_SERVER);
//                        }
//                    else if (!(miCheck->check->isMarked() && miCheck->value=="1") ||
//                        (miCheck->check->isMarked() && miCheck->value=="2"))
//                        {
//                          chatWindow->chatLog("Secenek "+toString(sayac) +" yanlis isaretlendi", BY_SERVER);
//                          tru--;
//                        }
//                }
//                chatWindow->chatLog("Sonuc = "+toString(tru), BY_SERVER);
//// tru + i->size() diyerek her yanlis sayısına göre değerlendirme yapılabilir
//                tru<0 ? tru=0: tru++;
                Net::getNpcHandler()->listInput(current_npc, 1);
//                Net::getNpcHandler()->nextDialog(current_npc);
            }
        }
        else  //çok sorulu
        {
        testState = SHOW_RESULT_STATE;
        Net::getNpcHandler()->listInput(current_npc, 2);
//        Net::getNpcHandler()->nextDialog(current_npc);
        }
        start();
    }else if (event.getId() == "radiocheck"&&mTotalQuestion>1)
    {
        int answer=2;  //boş bırakılmışsa yanlış kabul et
        for (miRadio = mvRadio.begin();
            miRadio != mvRadio.end();
            ++miRadio)
        {
            if (miRadio->radio->isSelected()) answer=miRadio->value;
        }
    Net::getNpcHandler()->listInput(current_npc, mTotalQuestion+answer+2);
    }
    else if (event.getId().substr(0,13) == "checkboxcheck" && mTotalQuestion == 1)
    {
        std::string tempstr2 = event.getId().substr(13,14);
        std::istringstream tempstr(tempstr2);
        int answer;
        tempstr >> answer;
        Net::getNpcHandler()->listInput(current_npc, answer + 1);
    }
    else if (event.getId() == "cancel")
    {
        choice = 0xff; // 0xff means cancel
        Net::getNpcHandler()->nextDialog(current_npc);
        current_npc = 0;
        setVisible(false);
        NPC::isTalking = false;
        reset();
    }
    else if (ch.substr(0,4)=="goto")
    {
        int i= atoi(ch.substr(5,4).c_str());
        Net::getNpcHandler()->listInput(current_npc, i+2);
        current_npc=0;
        NPC::isTalking = false;
        reset();
    }
    else if (ch=="go_prev")
    {
        if (mQuestionNumber>1)
        {
//!!!!!!!!!!!!!!!!!!!!!!!!
        Net::getNpcHandler()->listInput(current_npc, mQuestionNumber+1);//iptal ve değerlendir'i geçmek için +2-1
        current_npc=0;
        NPC::isTalking = false;
        reset();
        }
    }    else if (ch=="go_next")
    {
        if (mQuestionNumber<mTotalQuestion)
        {
//!!!!!!!!!!!!!!!!!!
        Net::getNpcHandler()->listInput(current_npc, mQuestionNumber+3); //iptal ve bişeyleri geçmek için +3
        current_npc=0;
        NPC::isTalking = false;
        reset();
        }
    }
    else if (event.getId()=="startcancel")
    {
        Net::getNpcHandler()->listInput(current_npc, 1);
        setVisible(false);
        reset();
        current_npc=0;
        NPC::isTalking = false;
    }
    else if (event.getId()=="startok")
    {
        mBasla = true;
        hideStart();
        showQuestion();
        Net::getNpcHandler()->listInput(current_npc, 2); //ilk menü
        mEvaluate->setVisible(true);
    }
    else if (event.getId()=="mesajok")
    {
        Net::getNpcHandler()->nextDialog(current_npc);
        setVisible(false);
        hideMessage();
        current_npc=0;
    }
}

void
TestDialog::setDoc(std::string xmlDoc)
{
    mDoc="";
    mDoc=xmlDoc;
}

void
TestDialog::parse()
{
    ResourceManager *resman = ResourceManager::getInstance();
    mxmlDoc=xmlParseMemory(mDoc.c_str(),mDoc.size());
    if (!mxmlDoc)
    {
        logger->error("testwindow.cpp: Error while parsing test xml (from npc.xml)!"+mDoc);
    }

    xmlNodePtr rootNode = xmlDocGetRootElement(mxmlDoc);
    if (!rootNode || !xmlStrEqual(rootNode->name, BAD_CAST "test"))
    {
        logger->error("testwindow.cpp: from npc xml is not a valid database file!"+mDoc);
    }

    for_each_xml_child_node(node, rootNode)
    {
        // Testten önce gösterilen mesaj
        if (xmlStrEqual(node->name, BAD_CAST "mesaj"))
        {
            testState = MESSAGE_STATE;
            mMessageText->clearRows();

            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST "addrow"))
                {
                    mMessageText->addRow(toTurkish(XML::getProperty(subnode, "text", "\n")));
                }
                else if (xmlStrEqual(subnode->name, BAD_CAST "effect"))
                {
                    std::string effecttype= XML::getProperty(subnode, "type", "particle");
                    std::string effectname= XML::getProperty(subnode, "name", "dogru1");
                    std::string effectsound= XML::getProperty(subnode, "sound", "dogru1");
                    makeEffect(effecttype,effectname,effectsound);
                }
            }
        }
        else if (xmlStrEqual(node->name, BAD_CAST "starter"))
        {
            // teste başlarken yapılması gerekenleri burada yap
            mQuestionNumber = 0;
            mBitti = false;
            mFinishScroll->setVisible(false);
            mBasla = false;
            deleteQuestionButtons();
            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST "window"))
                {
                    int w =  XML::getProperty(subnode, "width", 350);
                    int h =  XML::getProperty(subnode, "height", 275);
                    int l =  XML::getProperty(subnode, "left", (1024-getWidth())/2);
                    int t =  XML::getProperty(subnode, "top", (768-getHeight())/2);
                    setContentSize(w, h);
                    setPosition(l,t);
                    mFinishClose->setVisible(false);
                    mEvaluate->setPosition(40,getHeight()-100);
                }
                else if (xmlStrEqual(subnode->name, BAD_CAST "testpro"))
                {
                    mTotalQuestion = XML::getProperty(subnode, "totalquestion", 1); //belirtilmezse tek soru
                    mTotalTime = XML::getProperty(subnode, "totaltime", 0);
                    mSuccessLimit = XML::getProperty(subnode, "successlimit", 0);
                    mPunish = XML::getProperty(subnode, "punish", 0);
                    mAward = XML::getProperty(subnode, "award", 0);
                    //todo: dereceli odul  ceza için buraya ekleme yapılacak
                    if (mTotalQuestion==1)  testState = ONEQ_START_STATE;
                    else testState = MANYQ_START_SATATE;

                }
            }
             setupPositionInfoLabels();
        }
        else if (xmlStrEqual(node->name, BAD_CAST "question"))
        {
            mQuestionNumber = XML::getProperty(node, "qn", 1); //belirtilmezse 1. soru
            mType = XML::getProperty(node, "type", "radio");  //belirtilmezse radio
            mSelected= XML::getProperty(node, "selected", 0); //secili degil
            if (mType == "radio" && mTotalQuestion == 1)
                testState = ONE_RADIO_STATE;
            else if (mType == "check" && mTotalQuestion == 1)
                testState = ONE_CEHCK_STATE;
            else if (mTotalQuestion >1)
                testState = MANYQ_CONTINUE_STATE;
        }
        else if (xmlStrEqual(node->name, BAD_CAST "label"))
        {
            gcn::Label *templabel=new gcn::Label("");
            templabel->setCaption(XML::getProperty(node, "text", "label"));
            templabel->setX(XML::getProperty(node, "x", 0)+padX);
            templabel->setY(XML::getProperty(node, "y", 0)+padY);
            int w=XML::getProperty(node, "width", 0);
            int h=XML::getProperty(node, "height", 0);
            if (w==0||h==0)
                templabel->adjustSize();
              else
                templabel->setSize(w,h);

            int r = XML::getProperty(node, "fcolorr", 0);
            int g = XML::getProperty(node, "fcolorg", 0);
            int b = XML::getProperty(node, "fcolorb", 0);
            if (r!=0 && g!=0 && b!=0) templabel->setForegroundColor(gcn::Color(r,g,b));

            r = XML::getProperty(node, "bcolorr", 0);
            g = XML::getProperty(node, "bcolorg", 0);
            b = XML::getProperty(node, "bcolorb", 0);
            if (r!=0 && g!=0 && b!=0) templabel->setBackgroundColor(gcn::Color(r,g,b));

            std::string font = XML::getProperty(node, "font", "");
            if (font=="speechFont") templabel->setFont(boldFont);
             else if (font=="hitRedFont") templabel->setFont(boldFont);
             else if (font=="hitBlueFont") templabel->setFont(boldFont);
             else if (font=="hitYellowFont") templabel->setFont(boldFont);
             else templabel->setFont(boldFont);

//            templabel->setBorderSize(XML::getProperty(node, "bordersize", 0));
            templabel->setVisible(false);
            templabel->adjustSize();
            add(templabel);
            gcn::Label *t;
            t =templabel;
            mvLabel.push_back(t);

            templabel->setCaption(toTurkish(templabel->getCaption()));
        }
        else if (xmlStrEqual(node->name, BAD_CAST "image"))
        {
            SmImage temp;
            temp.img = resman->getImage(XML::getProperty(node, "src", ""));
            temp.x   =  XML::getProperty(node, "x", 0)+padX;
            temp.y   =  XML::getProperty(node, "y", 0)+padY;
            temp.visible = false;
            mvImage.push_back(temp);
        }

        else if (xmlStrEqual(node->name, BAD_CAST "simpleanim"))
        {
            SmAnim temp;
            ImageSet *mImageSet = resman->getImageSet(XML::getProperty(node, "src",""),
                                            XML::getProperty(node, "width", 0),
                                            XML::getProperty(node, "height", 0));

            Animation *mAnimation = new Animation();

            for (unsigned int i = 0; i < mImageSet->size(); ++i)
            {
                mAnimation->addFrame(mImageSet->get(i), 75, 0, 0);
            }
            temp.anim = new SimpleAnimation(mAnimation);
            temp.x   =  XML::getProperty(node, "x", 0)+padX;
            temp.y   =  XML::getProperty(node, "y", 0)+padY;
            temp.v   =  XML::getProperty(node, "v", 0);
            temp.visible = false;
            mvAnim.push_back(temp);
        }
// iki sütundan birisini seçerek yapılan karşılaştırma soruları eklenebilir
// xml'e grup değeri eklemek yeterli olur
        else if (xmlStrEqual(node->name, BAD_CAST "radio"))
        {
            SmRadio temp;
            std::string text = XML::getProperty(node, "text", "");
            text = toTurkish(text);
            int x = XML::getProperty(node, "x", 0)+padX;
            int y = XML::getProperty(node, "y", 0)+padY;

            gcn::RadioButton *mRadio = new RadioButton(text,"1",false);
            mRadio->setPosition(x, y);
            mRadio->setVisible(false);
            mRadio->addActionListener(this);
            const std::string &olayId = "radiocheck";
            mRadio->setActionEventId(olayId);
            add(mRadio);

            temp.value = XML::getProperty(node, "value", 0);
            if (temp.value==mSelected) mRadio->setSelected(true);
            temp.radio = mRadio;
            mvRadio.push_back(temp);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "check"))
        {
            SmCheck temp;
            std::string text = XML::getProperty(node, "text", "");
            text = toTurkish(text);
            int x = XML::getProperty(node, "x", 0)+padX;
            int y = XML::getProperty(node, "y", 0)+padY;

            CheckBox *mCheck = new CheckBox(text,false);
            mCheck->setPosition(x, y);
            mCheck->setVisible(false);

            temp.value = XML::getProperty(node, "value", "2");
            int sayi = XML::getProperty(node, "checked", -1);
            if (sayi==-1)
                mCheck->setSelected(false);
            else
                mCheck->setSelected(true);
            mCheck->addActionListener(this);
            const std::string &olayId = "checkboxcheck"+ temp.value;
            mCheck->setActionEventId(olayId);
            add(mCheck);

            temp.check = mCheck;
            mvCheck.push_back(temp);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "textbox"))
        {
            BrowserBox *mBrowserBox = new BrowserBox();
            ScrollArea *mScrollArea = new ScrollArea(mBrowserBox);
//            mScrollArea->setVerticalScrollPolicy(XML::getProperty(node, "scrollv",2));
//            mScrollArea->setHorizontalScrollPolicy(XML::getProperty(node, "scrollh",2));
            mScrollArea->setVerticalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
            mScrollArea->setHorizontalScrollPolicy(gcn::ScrollArea::SHOW_ALWAYS);
            int w =  XML::getProperty(node, "width", 350);
            int h =  XML::getProperty(node, "height", 275);
            int l =  XML::getProperty(node, "x", (800-getWidth())/2);
            int t =  XML::getProperty(node, "y", (600-getHeight())/2);
            mScrollArea->setOpaque(XML::getProperty(node, "opaque", true));
            mBrowserBox->setOpaque(XML::getProperty(node, "opaque", true));
            mScrollArea->setDimension(gcn::Rectangle(l, t, w, h ));
            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST "addrow"))
                {
                    std::string temprow = XML::getProperty(subnode, "text", "\n");
                    temprow = toTurkish(temprow);
                    mBrowserBox->addRow(temprow);
                }
            }
            add(mScrollArea);
            mvScrollArea.push_back(mScrollArea);
            mvTextBox.push_back(mBrowserBox);
        }
        else if (xmlStrEqual(node->name, BAD_CAST "report"))
        {
            testState = SHOW_RESULT_STATE;
            int mDogru = XML::getProperty(node, "ok", 0);
            std::string mmMesaj = XML::getProperty(node, "mesaj", "aa");
            hideEverything();
            mFinishScroll->setX((getWidth()-mFinishScroll->getWidth())/2);
            mFinishText->clearRows();
            mFinishText->addRow("    ##1      -= Test BİTTİ! =-");
            mFinishText->addRow("##2----------------------------------");
            mFinishText->addRow("Doğru Cevap:"+toString(mDogru));
            mFinishText->addRow("Mesaj      :"+mmMesaj);
            mFinishText->addRow("##2----------------------------------");
            mFinishText->addRow("##3   SN     Doğru C.    Sizin C.   ");
            mFinishText->addRow("##2  ----    --------    --------   ");
            for_each_xml_child_node(subnode, node)
            {
                if (xmlStrEqual(subnode->name, BAD_CAST "question"))
                {
                    std::string sh=" ABCDEFGHIJK";
                    std::stringstream ss;
                    int qn = XML::getProperty(subnode, "qn", 0);
                    ss<<"   ##8"<< qn;
                    if (qn<10) ss << " ";
                    ss<<"         ##9";
                    ss<<sh.at(XML::getProperty(subnode, "trueanswer", 0));
                    ss<<"           ##6";
                    ss<<sh.at(XML::getProperty(subnode, "studentanswer", 0));
                    mFinishText->addRow(ss.str());
                }
            }
        }
    }
    start();
}

void TestDialog::makeEffect(std::string type,std::string name, std::string ssound)
{
    if (type=="particle")
    {
        Particle *dogruFX;
        dogruFX = particleEngine->addEffect("graphics/particles/"+name+".particle.xml", 0, 0);
        player_node->controlParticle(dogruFX);
    }
    else
    {
//        particleEngine->addTextSplashEffect(name,
//                                    boldFont,
//                                    gcn::Color(255, 0, 255),
//                                    player_node->getPixelX() + 16,
//                                    player_node->getPixelY() + 16);
    particleEngine->addTextSplashEffect(name,
                                        player_node->getPixelX() + 16,
                                        player_node->getPixelY() + 16,
                                        &gcn::Color(255, 0, 255),
                                        boldFont);
    }
    if (ssound !="")
    {
        sound.playSfx("sfx/"+ssound+".ogg");
    }

}


std::string TestDialog::toTurkish(std::string str)
{
    std::string temp = str;
    for(int pos = 0; pos<str.length(); pos++)
    {
        if ((str.at(pos) == 'Ã') && (str.at(pos+1)=='¼')) temp = str.replace(pos, 2, "ü");
        if ((str.at(pos) == 'Å') && (str.at(pos+1)=='Ÿ')) temp = str.replace(pos, 2, "ş");
        if ((str.at(pos) == 'Ä') && (str.at(pos+1)=='Ÿ')) temp = str.replace(pos, 2, "ğ");
        if ((str.at(pos) == 'Ä') && (str.at(pos+1)=='±')) temp = str.replace(pos, 2, "ı");
        if ((str.at(pos) == 'Ã') && (str.at(pos+1)=='§')) temp = str.replace(pos, 2, "ç");
        if ((str.at(pos) == 'Ã') && (str.at(pos+1)=='¶')) temp = str.replace(pos, 2, "ö");

        if ((str.at(pos) == 'Ã') && (str.at(pos+1)=='œ')) temp = str.replace(pos, 2, "Ü");
        if ((str.at(pos) == 'Ä') && (str.at(pos+1)=='')) temp = str.replace(pos, 2, "Ğ");
        if ((str.at(pos) == 'Ä') && (str.at(pos+1)=='°')) temp = str.replace(pos, 2, "İ");
        if ((str.at(pos) == 'Å') && (str.at(pos+1)=='')) temp = str.replace(pos, 2, "Ş");
        if ((str.at(pos) == 'Ã') && (str.at(pos+1)=='‡')) temp = str.replace(pos, 2, "Ç");
        if ((str.at(pos) == 'Ã') && (str.at(pos+1)=='–')) temp = str.replace(pos, 2, "Ö");

        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='0')) temp = str.replace(pos, 3, "á");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='1')) temp = str.replace(pos, 3, "Á");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='2')) temp = str.replace(pos, 3, "é");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='3')) temp = str.replace(pos, 3, "É");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='4')) temp = str.replace(pos, 3, "í");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='5')) temp = str.replace(pos, 3, "Í");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='6')) temp = str.replace(pos, 3, "ó");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='7')) temp = str.replace(pos, 3, "Ó");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='8')) temp = str.replace(pos, 3, "ú");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='$')&& (str.at(pos+2)=='9')) temp = str.replace(pos, 3, "Ú");

        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='0')) temp = str.replace(pos, 3, "ë");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='1')) temp = str.replace(pos, 3, "¥");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='2')) temp = str.replace(pos, 3, "£");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='3')) temp = str.replace(pos, 3, "¢");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='4')) temp = str.replace(pos, 3, "¡");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='5')) temp = str.replace(pos, 3, "¿");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='6')) temp = str.replace(pos, 3, "à");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='7')) temp = str.replace(pos, 3, "ã");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='8')) temp = str.replace(pos, 3, "õ");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='!')&& (str.at(pos+2)=='9')) temp = str.replace(pos, 3, "ê");

        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='1')) temp = str.replace(pos, 3, "ñ");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='2')) temp = str.replace(pos, 3, "Ñ");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='3')) temp = str.replace(pos, 3, "ä");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='4')) temp = str.replace(pos, 3, "Ä");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='5')) temp = str.replace(pos, 3, "ß");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='6')) temp = str.replace(pos, 3, "ø");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='7')) temp = str.replace(pos, 3, "è");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='8')) temp = str.replace(pos, 3, "È");
        if ((str.at(pos) == '$') && (str.at(pos+1)=='%')&& (str.at(pos+2)=='9')) temp = str.replace(pos, 3, "å");

    }
   return temp;
}

