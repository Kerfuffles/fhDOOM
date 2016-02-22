#include "LightEditor.h"
#include <qlayout.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qcolordialog.h>


fhLightEditor::fhLightEditor(QWidget* parent)
: QDialog(parent) {

	QVBoxLayout* mainLayout = new QVBoxLayout;
	QHBoxLayout* leftRightLayout = new QHBoxLayout;

	QWidget* left = new QWidget;
	QVBoxLayout* leftLayout = new QVBoxLayout;
	left->setLayout(leftLayout);
	leftRightLayout->addWidget(left);

	m_lighttype = new QComboBox;
	m_lighttype->addItem("Point Light");
	m_lighttype->addItem("Parallel Light");
	m_lighttype->addItem("Projected Light");

	leftLayout->addWidget(m_lighttype);
	leftLayout->addWidget(CreatePointLightParameters());
	leftLayout->addWidget(CreateParallelLightParameters());
	leftLayout->addWidget(CreateProjectedLightParameters());

	QWidget* right = new QWidget;
	QVBoxLayout* rightLayout = new QVBoxLayout;
	right->setLayout( rightLayout );
	leftRightLayout->addWidget( right );
	
	m_castShadows = new QCheckBox("Cast Shadows");	
	rightLayout->addWidget(m_castShadows);	

	m_colorButton = new QPushButton(this);
	auto colorLayout = new QHBoxLayout;
	colorLayout->addWidget(new QLabel("Color"));
	colorLayout->addWidget(m_colorButton);
	rightLayout->addLayout(colorLayout);

	mainLayout->addLayout(leftRightLayout);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	m_cancelButton = new QPushButton("Cancel", this);
	m_applyButton = new QPushButton("Apply", this);
	m_okButton = new QPushButton("OK", this);	

	buttonLayout->addWidget(m_cancelButton);
	buttonLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
	buttonLayout->addWidget(m_applyButton);
	buttonLayout->addWidget(m_okButton);
	mainLayout->addLayout(buttonLayout);

	this->setLayout(mainLayout);

	QObject::connect(m_lighttype, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](){
		this->UpdateLightParameters(); 
	});

	QObject::connect(m_colorButton, &QPushButton::clicked, [=](){ 
		QColor color;
		color.setRgbF( m_currentData.color.x, m_currentData.color.y, m_currentData.color.z );

		QColorDialog dialog(color, this);
		dialog.setWindowModality(Qt::WindowModal);
		QObject::connect(&dialog, &QColorDialog::currentColorChanged, [&](){
			this->setLightColor(dialog.currentColor());	
			this->UpdateGame();
		});

		dialog.exec();
	});

	QObject::connect(m_pointlightParameters.radius, &fhVec3Edit::valueChanged, [=](idVec3 v){
		this->m_currentData.radius = v;
		this->UpdateGame();
	} );

	QObject::connect( m_pointlightParameters.center, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.center = v;
		this->UpdateGame();
	} );

	QObject::connect( m_parallellightParameters.radius, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.radius = v;
		this->UpdateGame();
	} );

	QObject::connect( m_parallellightParameters.direction, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.center = v;
		this->UpdateGame();
	} );

	QObject::connect( m_projectedlightParameters.target, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.target = v;
		this->UpdateGame();
	} );

	QObject::connect( m_projectedlightParameters.right, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.right = v;
		this->UpdateGame();
	} );

	QObject::connect( m_projectedlightParameters.up, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.up = v;
		this->UpdateGame();
	} );

	QObject::connect( m_projectedlightParameters.start, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.start = v;
		this->UpdateGame();
	} );

	QObject::connect( m_projectedlightParameters.end, &fhVec3Edit::valueChanged, [=]( idVec3 v ){
		this->m_currentData.end = v;
		this->UpdateGame();
	} );

	QObject::connect( m_castShadows, &QCheckBox::stateChanged, [=](){
		this->m_currentData.castShadows = this->m_castShadows->isChecked();
		this->UpdateGame();
	} );

	UpdateLightParameters();
}

fhLightEditor::~fhLightEditor() {
}

QWidget* fhLightEditor::CreatePointLightParameters() {		

	QGroupBox* pointlightGroup = new QGroupBox( tr( "Point Light Parameters" ) );;
	QGridLayout* pointlightgrid = new QGridLayout;	

	m_pointlightParameters.radius = new fhVec3Edit(fhVec3Edit::Size, true);
	m_pointlightParameters.center = new fhVec3Edit(fhVec3Edit::Position);

	pointlightgrid->addWidget( new QLabel( "Radius" ),        0, 0, Qt::AlignBottom );
	pointlightgrid->addWidget( m_pointlightParameters.radius, 0, 1 );

	pointlightgrid->addWidget( new QLabel( "Center" ),        1, 0, Qt::AlignBottom );
	pointlightgrid->addWidget( m_pointlightParameters.center, 1, 1 );

	pointlightGroup->setLayout( pointlightgrid );

	return pointlightGroup;
}

QWidget* fhLightEditor::CreateParallelLightParameters() {

	QGroupBox* group = new QGroupBox( tr( "Parallel Light Parameters" ) );;
	QGridLayout* grid = new QGridLayout;	

	m_parallellightParameters.radius = new fhVec3Edit(fhVec3Edit::Size, true);
	m_parallellightParameters.direction = new fhVec3Edit(fhVec3Edit::Direction);

	grid->addWidget( new QLabel( "Radius" ), 0, 0, Qt::AlignBottom );
	grid->addWidget( m_parallellightParameters.radius, 0, 1 );

	grid->addWidget( new QLabel( "Center" ), 1, 0, Qt::AlignBottom );
	grid->addWidget( m_parallellightParameters.direction, 1, 1 );	

	group->setLayout( grid );

	return group;
}

QWidget* fhLightEditor::CreateProjectedLightParameters() {

	QGroupBox* group = new QGroupBox( tr( "Projected Light Parameters" ) );;
	QGridLayout* grid = new QGridLayout;	

	m_projectedlightParameters.target = new fhVec3Edit(fhVec3Edit::Direction, true);
	m_projectedlightParameters.right = new fhVec3Edit(fhVec3Edit::Direction);
	m_projectedlightParameters.up = new fhVec3Edit(fhVec3Edit::Direction);
	m_projectedlightParameters.start = new fhVec3Edit(fhVec3Edit::Direction, true);
	m_projectedlightParameters.end = new fhVec3Edit(fhVec3Edit::Direction);

	grid->addWidget( new QLabel( "Target" ), 0, 0, Qt::AlignBottom );
	grid->addWidget( m_projectedlightParameters.target, 0, 1 );

	grid->addWidget( new QLabel( "Right" ), 1, 0, Qt::AlignBottom );
	grid->addWidget( m_projectedlightParameters.right, 1, 1 );

	grid->addWidget( new QLabel( "Up" ), 2, 0, Qt::AlignBottom );
	grid->addWidget( m_projectedlightParameters.up, 2, 1 );

	m_projectedlightParameters.explicitStartEnd = new QCheckBox;
	m_projectedlightParameters.explicitStartEnd->setText("Explicit Start/End");
	grid->addWidget(m_projectedlightParameters.explicitStartEnd, 3, 0, 1, 2);

	grid->addWidget( new QLabel( "Start" ), 4, 0, Qt::AlignBottom );
	grid->addWidget( m_projectedlightParameters.start, 4, 1 );

	grid->addWidget( new QLabel( "End" ), 5, 0, Qt::AlignBottom );
	grid->addWidget( m_projectedlightParameters.end, 5, 1 );

	group->setLayout( grid );

	QObject::connect(m_projectedlightParameters.explicitStartEnd, &QCheckBox::stateChanged, [=](){
		this->UpdateLightParameters();
	} );

	return group;
}


void fhLightEditor::UpdateLightParameters() {
	m_currentData.type = static_cast<fhLightType>(m_lighttype->currentIndex());
	const bool pointlight = (m_lighttype->currentIndex() == 0);
	const bool parallellight = (m_lighttype->currentIndex() == 1);
	const bool projectedlight = (m_lighttype->currentIndex() == 2);
	const bool explicitStartEnd = projectedlight && m_projectedlightParameters.explicitStartEnd->isChecked();

	m_pointlightParameters.center->setEnabled(pointlight);
	m_pointlightParameters.radius->setEnabled(pointlight);

	m_parallellightParameters.direction->setEnabled(parallellight);
	m_parallellightParameters.radius->setEnabled(parallellight);

	m_projectedlightParameters.target->setEnabled( projectedlight );
	m_projectedlightParameters.up->setEnabled( projectedlight );
	m_projectedlightParameters.right->setEnabled( projectedlight );
	m_projectedlightParameters.explicitStartEnd->setEnabled( projectedlight );
	m_projectedlightParameters.start->setEnabled( explicitStartEnd );
	m_projectedlightParameters.end->setEnabled( explicitStartEnd );
}


void fhLightEditor::Data::initFromSpawnArgs( const idDict* spawnArgs ) {
	if (spawnArgs->GetVector( "light_right", "-128 0 0", right )) {
		type = fhLightType::Projected;
	}
	else if (spawnArgs->GetInt( "parallel", "0" ) != 0) {
		type = fhLightType::Parallel;
	}
	else {
		type = fhLightType::Point;
	}

	target = spawnArgs->GetVector( "light_target", "0 0 -256" );
	up = spawnArgs->GetVector( "light_up", "0 -128 0" );
	start = spawnArgs->GetVector( "light_start", "0 0 0" );
	end = spawnArgs->GetVector( "light_end", "0 0 0" );
	radius = spawnArgs->GetVector( "light_radius", "100 100 100" );
	center = spawnArgs->GetVector( "light_center", "0 0 0" );
	color = spawnArgs->GetVector( "_color", "1 1 1" );
	castShadows = !spawnArgs->GetBool("noshadows");
}

void fhLightEditor::Data::toSpawnArgs(idDict* spawnArgs) {	

	spawnArgs->Delete("light_right");
	spawnArgs->Delete("light_up");
	spawnArgs->Delete("light_target");
	spawnArgs->Delete("_color");
	spawnArgs->Delete("light_center");
	spawnArgs->Delete("light_radius");
	spawnArgs->Delete("light_start");
	spawnArgs->Delete("light_end");
	spawnArgs->Delete("parallel");
	spawnArgs->Delete("noshadows");
	spawnArgs->Delete("nospecular");
	spawnArgs->Delete("nodiffuse");

	switch(type) {
	case fhLightType::Projected:
		spawnArgs->SetVector("light_right", right);
		spawnArgs->SetVector("light_up", up);
		spawnArgs->SetVector("light_target", target);
		spawnArgs->SetVector("light_start", start);
		spawnArgs->SetVector("light_end", end);
		break;
	case fhLightType::Parallel:
		spawnArgs->SetInt("parallel", 1);
		spawnArgs->SetVector( "light_radius", radius );
		spawnArgs->SetVector( "light_center", center );
		break;
	case fhLightType::Point:
		spawnArgs->SetInt("parallel", 0);
		spawnArgs->SetVector( "light_radius", radius );
		spawnArgs->SetVector( "light_center", center );
		break;
	};

	spawnArgs->SetVector("_color", color);	
	spawnArgs->SetBool("noshadows", !castShadows);

	//TODO(johl): are expected to be there(?), but are not used by the game
	spawnArgs->SetBool("nospecular", false);
	spawnArgs->SetBool("nodiffuse", false);
	spawnArgs->SetFloat("falloff", 0.0f);
}



static void initVec3EditFromSpawnArg(fhVec3Edit* edit, const idDict* spawnArgs, const char* name, const char* defaultString) {
	idVec3 v = spawnArgs->GetVector(name, defaultString);
	edit->set(v);
}


void fhLightEditor::initFromSpawnArgs(const idDict* spawnArgs) {
	if(!spawnArgs)
		return;
	
	m_currentData.initFromSpawnArgs(spawnArgs);

	m_lighttype->setCurrentIndex((int)m_currentData.type);
	m_pointlightParameters.radius->set(m_currentData.radius);
	m_pointlightParameters.center->set(m_currentData.center);
	m_parallellightParameters.radius->set( m_currentData.radius );
	m_parallellightParameters.direction->set( m_currentData.center );
	m_projectedlightParameters.target->set( m_currentData.target );
	m_projectedlightParameters.right->set( m_currentData.right );
	m_projectedlightParameters.up->set( m_currentData.up );
	m_projectedlightParameters.start->set( m_currentData.start );
	m_projectedlightParameters.end->set( m_currentData.end );	
	m_castShadows->setChecked(m_currentData.castShadows);

	QColor color;
	color.setRgbF(m_currentData.color.x, m_currentData.color.y, m_currentData.color.z);
	setLightColor(color);
}

void fhLightEditor::setLightColor(QColor color) {

	QString s( "background: #"
		+ QString( color.red() < 16 ? "0" : "" ) + QString::number( color.red(), 16 )
		+ QString( color.green() < 16 ? "0" : "" ) + QString::number( color.green(), 16 )
		+ QString( color.blue() < 16 ? "0" : "" ) + QString::number( color.blue(), 16 ) + ";" );
	m_colorButton->setStyleSheet( s );
	m_colorButton->update();

	QRgb rgb = color.rgb();
	m_currentData.color.x = qRed(rgb) / 255.0f;
	m_currentData.color.y = qGreen(rgb) / 255.0f;
	m_currentData.color.z = qBlue(rgb) / 255.0f;
}

void fhLightEditor::UpdateGame() {
	idEntity* list[128];	
	int count = gameEdit->GetSelectedEntities( list, 128 );	

	idDict newSpawnArgs;
	m_currentData.toSpawnArgs( &newSpawnArgs );

	for(int i=0; i<count; ++i) {
		gameEdit->EntityChangeSpawnArgs(list[i], &newSpawnArgs);
		gameEdit->EntityUpdateChangeableSpawnArgs( list[i], NULL );
		gameEdit->EntityUpdateVisuals(list[i]);
	}

	//gameEdit->FindEntity()
}