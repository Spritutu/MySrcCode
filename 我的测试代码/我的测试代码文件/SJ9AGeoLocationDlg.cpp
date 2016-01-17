#include "SJ9AGeoLocationDlg.h"
#include "Common.h"

#include <QFileDialog>
#include <QMessageBox>

#include "ImageCut.h"
#include "SJ9SensorModel.h"

CSJ9AGeoLocationDlg::CSJ9AGeoLocationDlg(QWidget* parent)
: QDialog( parent )
{
	setupUi(this);
	SetDialogStyle(this);

	QObject::connect(pushButtonInputEhp, SIGNAL(clicked()), this, SLOT(slotInputEph()));
	QObject::connect(pushButtonInputPnt, SIGNAL(clicked()), this, SLOT(slotInputPoint()));
	QObject::connect(pushButtonInputImg, SIGNAL(clicked()), this, SLOT(slotInputImg()));
	QObject::connect(pushButtonOutputImg, SIGNAL(clicked()), this, SLOT(slotOutputImg()));
	QObject::connect(pushButtonOk, SIGNAL(clicked()), this, SLOT(OnOK()));
	QObject::connect(pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	QObject::connect(checkBoxCalPt,SIGNAL(stateChanged(int)), this, SLOT(slotChanged(int)));

	m_strInputEph.clear();
	m_strInputPoint.clear();
	m_strInputImage.clear();
	m_strOutputImage.clear();
	m_bCalcPoint = true;
}

CSJ9AGeoLocationDlg::~CSJ9AGeoLocationDlg()
{
}

void CSJ9AGeoLocationDlg::slotInputEph()
{
	QString strDir = GetLastDirectory("input_sj9a_eph");
	QString fileName = QFileDialog::getOpenFileName(this,tr("��������eph·��"),strDir,tr("�����ļ�(*_eph.xml)"),0,0);

	if(!fileName.isEmpty())
	{
		lineEditInputEph->setText(fileName);
		SetLastDirectory("input_sj9a_eph", fileName);
	}
}

void CSJ9AGeoLocationDlg::slotInputPoint()
{
	QString strDir = GetLastDirectory("input_sj9a_point_csv");
	QString fileName = QFileDialog::getOpenFileName(this,tr("���������·��"), strDir,tr("�����ļ�(*.csv)"),0,0);

	if(!fileName.isEmpty())
	{
		lineEditInputPnt->setText(fileName);
		SetLastDirectory("input_sj9a_point_csv",fileName);
	}

	m_strInputPoint = QFile::encodeName( lineEditInputPnt->text() ).constData();

	ReadPointFile();	//��ȡ���ļ�
	ShowList();	// ˢ���б�
}

void CSJ9AGeoLocationDlg::slotInputImg()
{
	QString strDir = GetLastDirectory("input_sj9a_img");

	QString selectedFilter;
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("����ͼ��·��"),
		strDir,
		tr("����ͼ���ʽ (*.vrt);;GeoTiff (*.tif *.tiff);;Erdas Imagine (*.img);;PCIDSK (*.pix);;ENVI (*.*);;�����ļ�(*.*)"),
		&selectedFilter, 0);

	if(!fileName.isEmpty())
	{
		lineEditInputImg->setText(fileName);
		SetLastDirectory("input_sj9a_img", fileName);
	}
}

void CSJ9AGeoLocationDlg::slotOutputImg()
{
	QString strDir = GetLastDirectory("output_sj9a_img");

	QString selectedFilter;
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("�������Ŀ¼"),
		strDir,
		tr("Erdas Imagine (*.img);;GeoTiff (*.tif);;PCIDSK (*.pix);;ENVI (*.hdr)"),
		&selectedFilter, 0);

	if(!CheckOutputFile(lineEditOutputImg, fileName))
		QMessageBox::warning(this, tr("��ʾ"),	tr("ָ�����ļ����ڱ�ʹ�ã����ܸ��Ǵ��ļ�����ָ�������ļ�����"), QMessageBox::Ok );

	if(!fileName.isEmpty())
	{
		lineEditOutputImg->setText(fileName);
		SetLastDirectory("output_sj9a_img",fileName);
	}
}

void CSJ9AGeoLocationDlg::ReadPointFile()
{
	// ��m_vPoints��ֵ
	m_vPoints.clear();
	QString fileName = QString::fromStdString(m_strInputPoint);

	if (fileName.isEmpty()) 
	{
		QMessageBox::information(this, tr(" ��ʾ"), tr("�ļ�·��Ϊ�գ�"));
		return;
	}

	QFile file(fileName);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::information(this, tr(" ��ʾ"), tr("�ļ����ܴ򿪣�"));
		return;
	}

	QTextStream stream(&file);
	QString line;

	int row = 0;
	do 
	{
		line = stream.readLine();
		if (!line.isEmpty()) 
		{
			QStringList pieces = line.split(",", QString::SkipEmptyParts);
			PointDataStruct point;
			point.m_iLineStart = pieces.value(0).toInt();
			point.m_iLineColum = pieces.value(1).toInt();
			point.m_dLineTime = pieces.value(2).toDouble();
			point.m_dLon = pieces.value(3).toDouble();
			point.m_dLat = pieces.value(4).toDouble();
			m_vPoints.push_back(point);
		}
	} while (!line.isEmpty());

	file.close();
}

void CSJ9AGeoLocationDlg::CreateItem(int index, int icol, int iValue)
{
	QTableWidgetItem *pItem = new QTableWidgetItem();
	pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
	pItem->setData(0, iValue);
	tableWidgetPoint->setItem(index, icol, pItem);
}

void CSJ9AGeoLocationDlg::CreateItem(int index, int icol, double dValue)
{
	QTableWidgetItem *pItem = new QTableWidgetItem();
	pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
	pItem->setData(0, dValue);
	tableWidgetPoint->setItem(index, icol, pItem);
}

void CSJ9AGeoLocationDlg::ShowList()
{
	tableWidgetPoint->clear();
	tableWidgetPoint->setColumnCount(10);

	QHeaderView *headerView = tableWidgetPoint->horizontalHeader();
	headerView->setStretchLastSection(true);

	QStringList itemLabels;
	itemLabels << tr("�к�") << tr("�к�") << tr("ʱ����") << tr("׼ȷ����") << tr("׼ȷγ��") << tr("���㾭��") << tr("����γ��") << tr("���Ȳ�") << tr("γ�Ȳ�") << tr("�����");
	tableWidgetPoint->setHorizontalHeaderLabels(itemLabels);

	int count = m_vPoints.size();
	tableWidgetPoint->setRowCount(count);//������

	for (int i=0; i<count ;i++)
	{
		CreateItem(i, 0, m_vPoints[i].m_iLineStart);
		CreateItem(i, 1, m_vPoints[i].m_iLineColum);
		CreateItem(i, 2, m_vPoints[i].m_dLineTime);
		CreateItem(i, 3, m_vPoints[i].m_dLon);
		CreateItem(i, 4, m_vPoints[i].m_dLat);
		CreateItem(i, 5, m_vPoints[i].m_dCalLon);
		CreateItem(i, 6, m_vPoints[i].m_dCalLat);
		CreateItem(i, 7, m_vPoints[i].m_dDeltaLon);
		CreateItem(i, 8, m_vPoints[i].m_dDeltaLat);
		CreateItem(i, 9, m_vPoints[i].m_dRMS);
	}

	tableWidgetPoint->resizeColumnsToContents();
	tableWidgetPoint->resizeRowsToContents();
}

int BuildCropVrt(const char *pszStripFile,const char *pszOutVrt,int iStartLine,int iCropHeight,int iWidth = 12208)
{

	FILE* pVRTFile = fopen(pszOutVrt, "wt");
	if (pVRTFile == NULL)
		return RE_FAILED;

	fprintf(pVRTFile, "<VRTDataset rasterXSize=\"%d\" rasterYSize=\"%d\">\n", iWidth, iCropHeight);
	fprintf(pVRTFile, "<VRTRasterBand dataType=\"UInt16\" band=\"%d\">\n<SimpleSource>\n", 1);
	fprintf(pVRTFile, "<SourceFilename relativeToVRT=\"0\">%s</SourceFilename>\n",pszStripFile);
	fprintf(pVRTFile, "<SourceBand>1</SourceBand>\n");
	fprintf(pVRTFile, "<SrcRect xOff=\"0\" yOff=\"%d\" xSize=\"%d\" ySize=\"%d\" />\n",iStartLine,iWidth,iCropHeight);
	fprintf(pVRTFile, "<DstRect xOff=\"0\" yOff=\"0\" xSize=\"%d\" ySize=\"%d\" />\n",iWidth,iCropHeight);
	fprintf(pVRTFile, "</SimpleSource>\n</VRTRasterBand>\n</VRTDataset>\n");
	fclose(pVRTFile);

	return RE_SUCCESS;
}

void CSJ9AGeoLocationDlg::OnOK()
{
	bool bPan = radioButtonPan->isChecked();
	m_bCalcPoint = checkBoxCalPt->isChecked();

	m_strInputEph = QFile::encodeName( lineEditInputEph->text() ).constData();
	m_strInputPoint = QFile::encodeName( lineEditInputPnt->text() ).constData();
	m_strInputImage = QFile::encodeName( lineEditInputImg->text() ).constData();
	m_strOutputImage = QFile::encodeName( lineEditOutputImg->text() ).constData();

	if (bPan)
	{
		if (m_bCalcPoint)
		{
			CProcessDlg *pPro = new CProcessDlg();
			pPro->setWindowTitle(tr("���ڼ��㶨λ���..."));
			pPro->show();

			pPro->SetStepCount(m_vPoints.size());
			pPro->SetPosition(0.0);

			for (size_t i=0; i<m_vPoints.size(); i++)
			{
				CSJ9SensorModel model(m_strInputEph.c_str(), m_vPoints[i].m_iLineStart);
				CPoint2D ptLineSample(m_vPoints[i].m_iLineColum-1, 0);
				CPoint3D ptLatlon(0,0,0);
				model.lineSampleToWorld(ptLineSample, ptLatlon);
				m_vPoints[i].m_dCalLon = ptLatlon.lon;
				m_vPoints[i].m_dCalLat = ptLatlon.lat;
				m_vPoints[i].m_dDeltaLon = (m_vPoints[i].m_dLon - m_vPoints[i].m_dCalLon) * 111120;
				m_vPoints[i].m_dDeltaLat = (m_vPoints[i].m_dLat - m_vPoints[i].m_dCalLat) * 111120;
				m_vPoints[i].m_dRMS = sqrt(m_vPoints[i].m_dDeltaLon*m_vPoints[i].m_dDeltaLon + m_vPoints[i].m_dDeltaLat*m_vPoints[i].m_dDeltaLat);

				pPro->StepIt();
			}

			ShowList();	//ˢ���б�

			delete pPro;
			QMessageBox::information(this,tr("��ʾ"), tr("�������"));	
		}
		else	//����ͼ��
		{
			QString strOutImage = lineEditOutputImg->text();
			if (IsFileOrDir(strOutImage))//�ļ�������
			{	
				QMessageBox::warning(this, tr("��ʾ"),	tr("ָ��������ļ��Ѿ����ڣ���ָ�������ļ���"), QMessageBox::Ok );
				return;
			}

			QDateTime startTime = QDateTime::currentDateTime();	//��ȡ���㿪ʼʱ��
			CProcessDlg *pPro = new CProcessDlg();
			pPro->setWindowTitle(tr("����ִ��SJ9A���ζ�λ"));
			pPro->show();

			int iRev = RE_SUCCESS;
			int iStartLine = spinBoxStartLine->value();

			QString strStartLine = QString("_%1").arg(iStartLine);
			string  strStartLineTmp = strStartLine.toStdString();

			// �Ȳ���һ��ͼ�����
			string strCutImage = m_strOutputImage;
			strCutImage = strCutImage.substr(0, strCutImage.rfind('.')) + strStartLineTmp + "_sub.vrt";

			BuildCropVrt(m_strInputImage.c_str(),strCutImage.c_str(),iStartLine,12208,12208);

		/*	Envelope cutExtent(0, 12207, iStartLine, iStartLine+12207);
	
			CImageCut cut(m_strInputImage.c_str(), pPro);
			cut.ExecuteCutImage(strCutImage.c_str(), &cutExtent);*/

			// �Բ��г�����ͼ�����У������
			CSJ9SensorModel model(m_strInputEph.c_str(), iStartLine);
		
			// �������������ļ�
			string strCoordGrid = m_strOutputImage;
			strCoordGrid = strCoordGrid.substr(0, strCoordGrid.rfind('.')) + "_grid.dat";
			model.ExportGrid(strCoordGrid.c_str(), 20, 20);

			// ����RPC�ļ�
			string strRPCFile = strCutImage;
			strRPCFile = strRPCFile.substr(0, strRPCFile.rfind('.')) + "_rpc.txt";
			model.ExportRPC(strRPCFile.c_str(), 20, 20, 1);

			// ʹ���ϸ���ģ��У��
			CSensorModelWarp warp(strCutImage.c_str(), m_strOutputImage.c_str(), &model, "GTiff", 1, pPro);
			iRev = warp.DoSensorModelBlockWarp(0, 0, RM_NearestNeighbour, /*4.0117647058823529411764705882353*/65535/255.0);

			delete pPro;

			QString strInfo = GetProcessInfomation(startTime, iRev);
			QMessageBox::information(this,tr("��ʾ"), strInfo);	
			accept();
		}
	}
	else
	{
	}
}

void CSJ9AGeoLocationDlg::slotChanged(int state)
{
	if (state == 0)
	{
		labelInputPnt->setEnabled(false);
		lineEditInputPnt->setEnabled(false);
		pushButtonInputPnt->setEnabled(false);
		labelInputImg->setEnabled(true);
		lineEditInputImg->setEnabled(true);
		pushButtonInputImg->setEnabled(true);
		labelOutputImg->setEnabled(true);
		lineEditOutputImg->setEnabled(true);
		pushButtonOutputImg->setEnabled(true);
		labelStartLine->setEnabled(true);
		spinBoxStartLine->setEnabled(true);
		tableWidgetPoint->setEnabled(false);
		this->setFixedSize(this->width(), 190);
	}
	else//ѡ��
	{
		labelInputPnt->setEnabled(true);
		lineEditInputPnt->setEnabled(true);
		pushButtonInputPnt->setEnabled(true);
		labelInputImg->setEnabled(false);
		lineEditInputImg->setEnabled(false);
		pushButtonInputImg->setEnabled(false);
		labelOutputImg->setEnabled(false);
		lineEditOutputImg->setEnabled(false);
		pushButtonOutputImg->setEnabled(false);
		labelStartLine->setEnabled(false);
		spinBoxStartLine->setEnabled(false);
		tableWidgetPoint->setEnabled(true);			
		this->setFixedSize(this->width(), 390);
	}
}