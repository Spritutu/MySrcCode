#include "BatToolDlg.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
using namespace std;

CBatReNameDlg::CBatReNameDlg(QWidget *parent):QDialog(parent)
{
	setupUi(this);
	progressBar->setValue(0);
	QObject::connect(pushButtonOpen,SIGNAL(clicked()),this,SLOT(OpenDir()));
	QObject::connect(pushButtonReName,SIGNAL(clicked()),this,SLOT(ReName()));
	QObject::connect(pushButtonQuit,SIGNAL(clicked()),this,SLOT(reject()));
	QPixmap pixmap("backimg.png");
	QPalette palette;
	palette.setBrush(this->backgroundRole(),QBrush(pixmap));
	this->setPalette(palette);
	//this->setMask(pixmap.mask());//��ͼƬ�е�͸��������ʾ����
	this->setAutoFillBackground(true);
}
CBatReNameDlg::~CBatReNameDlg()
{

}
void CBatReNameDlg::OpenDir()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

	QString directory = QFileDialog::getExistingDirectory(this,
		tr("���������ļ�·��"),lineEditDir->text(),options);

	if (!directory.isEmpty())
	{
		lineEditDir->setText(directory);
		m_strDir=directory.toStdString();
	}
}
void CBatReNameDlg::ReName()
{
	m_strDir=lineEditDir->text().toStdString();
	path dir(m_strDir);
	if (dir.empty())
	{
		QMessageBox::information(this,"��ʾ","����·��������",QMessageBox::Ok);
		return;
	}
	m_strExt=lineEdit_Ext->text().toStdString();
	m_strPre=lineEdit_Pre->text().toStdString();
	if (m_strExt=="")
	{

		QMessageBox::information(this,"��ʾ","������Ҫ���������ļ�����",QMessageBox::Ok);
		return;
	}
	vector<path>v;
	string strEx="."+m_strExt;
	label_pro->setText("���ڲ����ļ�");
	FindExtenFile(dir,v,strEx,true);
	int isize=v.size();
	if (isize==0)
	{
		label_pro->setText("����");
		QMessageBox::information(this,"��ʾ","δ�ҵ�ָ����ʽ���ļ�",QMessageBox::Ok);
		return;
	}
	progressBar->setRange(0,isize);
	int num=0;
	label_pro->setText("����������");
	pushButtonOpen->setEnabled(false);
	pushButtonReName->setEnabled(false);
	pushButtonQuit->setEnabled(false);
	for (int i=0;i<isize;i++)
	{
		progressBar->setValue(i+1);
		QString QstrName=QString("%1").arg(i);
		string strName=m_strPre+QstrName.toStdString()+"."+ m_strExt;
		path FileName(strName);
		path des=v[i].parent_path()/FileName;
		QString strInfoName=QString(des.string().c_str());
		QFileInfo qInfo(strInfoName);
		if (qInfo.exists()&&qInfo.isFile())
			continue;
		rename(v[i],des);
		num=num+1;
	}
	pushButtonOpen->setEnabled(true);
	pushButtonReName->setEnabled(true);
	pushButtonQuit->setEnabled(true);
	label_pro->setText("���������");
	QString str=QString("���ҵ��ļ�%1�����������ļ�%2��").arg(isize).arg(num);
	QMessageBox::information(this,"��ʾ",str,QMessageBox::Ok);
	//BatReName(v,m_strPre); //����ʵ�ֽ�����
}