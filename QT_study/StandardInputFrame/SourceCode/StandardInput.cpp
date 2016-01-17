#include "StandardInput.h"

CStandInput::CStandInput()
{
	setWindowTitle("��׼�����");

	label1=new QLabel("����");
	label2=new QLabel("�Ա�");
	label3=new QLabel("����");
	label4=new QLabel("���");

	labelName=new QLabel("����");
	labelName->setFrameStyle(QFrame::Panel|QFrame::Sunken);

	labelSex=new QLabel("��");
	labelSex->setFrameStyle(QFrame::Panel|QFrame::Sunken);

	labelAge=new QLabel("22");
	labelAge->setFrameStyle(QFrame::Panel|QFrame::Sunken);

	labelStature=new QLabel("175cm");
	labelStature->setFrameStyle(QFrame::Panel|QFrame::Sunken);

	nameButton=new QPushButton;
	nameButton->setIcon(QIcon("btn.png"));
	nameButton->setText("��������");

	sexButton=new QPushButton;
	sexButton->setIcon(QIcon("btn.png"));
	sexButton->setText("�����Ա�");

	ageButton=new QPushButton;
	ageButton->setIcon(QIcon("btn.png"));
	ageButton->setText("��������");

	statureButton=new QPushButton;
	statureButton->setIcon(QIcon("btn.png"));
	statureButton->setText("�������");

	QGridLayout *layout=new QGridLayout(this);

	connect(nameButton,SIGNAL(clicked()),this,SLOT(SetName()));
	connect(sexButton,SIGNAL(clicked()),this,SLOT(SetSex()));
	connect(ageButton,SIGNAL(clicked()),this,SLOT(SetAge()));
	connect(statureButton,SIGNAL(clicked()),this,SLOT(SetStature()));

	layout->addWidget(label1,0,0);
	layout->addWidget(labelName,0,1);
	layout->addWidget(nameButton,0,2);

	layout->addWidget(label2,1,0);
	layout->addWidget(labelSex,1,1);
	layout->addWidget(sexButton,1,2);

	layout->addWidget(label3,2,0);
	layout->addWidget(labelAge,2,1);
	layout->addWidget(ageButton,2,2);

	layout->addWidget(label4,3,0);
	layout->addWidget(labelStature,3,1);
	layout->addWidget(statureButton,3,2);

	resize(250,150);
}
CStandInput::~CStandInput()
{

}
void CStandInput::SetName()
{
	bool ok;
	QString name=QInputDialog::getText(this,"�û�����","�������û�����:",QLineEdit::Normal,
		labelName->text(),&ok);
	if (ok && !name.isEmpty())
		labelName->setText(name);
}
void CStandInput::SetSex()
{
	QStringList list;
	list<<"��"<<"Ů";
	bool ok;
	QString sex=QInputDialog::getItem(this,"�Ա�","��ѡ���Ա�:",list,0,false,&ok);
	if (ok)
		labelSex->setText(sex);
}
void CStandInput::SetAge()
{
	bool ok;
	int age=QInputDialog::getInteger(this,"�û�����","�������û�����:",
		labelAge->text().toInt(),0,150,1,&ok);
	if (ok)
		labelAge->setText(QString(tr("%1")).arg(age));
}
void CStandInput::SetStature()
{
	bool ok;
	int stature=QInputDialog::getDouble(this,"���","���������:",175.00,50.00,250.00,1,&ok);
	if (ok)
		labelStature->setText(QString(tr("%1cm")).arg(stature));
}