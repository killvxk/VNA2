#include "preferences.h"
#include "ui_preferencesdialog.h"
#include <QSettings>
#include <QPushButton>
#include <QMessageBox>
#include <map>

using namespace std;

PreferencesDialog::PreferencesDialog(Preferences *pref, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    p(pref)
{
    ui->setupUi(this);

    // Setup GUI connections and adjustments
    // Startup page
    connect(ui->StartupSweepLastUsed, &QPushButton::clicked, [=](){
       ui->StartupSweepStart->setEnabled(false);
       ui->StartupSweepStop->setEnabled(false);
       ui->StartupSweepPoints->setEnabled(false);
       ui->StartupSweepLevel->setEnabled(false);
       ui->StartupSweepBandwidth->setEnabled(false);
    });
    connect(ui->StartupSweepDefault, &QPushButton::clicked, [=](){
       ui->StartupSweepStart->setEnabled(true);
       ui->StartupSweepStop->setEnabled(true);
       ui->StartupSweepPoints->setEnabled(true);
       ui->StartupSweepLevel->setEnabled(true);
       ui->StartupSweepBandwidth->setEnabled(true);
    });
    ui->StartupSweepStart->setUnit("Hz");
    ui->StartupSweepStart->setPrefixes(" kMG");
    ui->StartupSweepStop->setUnit("Hz");
    ui->StartupSweepStop->setPrefixes(" kMG");
    ui->StartupSweepBandwidth->setUnit("Hz");
    ui->StartupSweepBandwidth->setPrefixes(" k");

    // Page selection
    connect(ui->treeWidget, &QTreeWidget::currentItemChanged, [=](QTreeWidgetItem *current, QTreeWidgetItem *) {
        auto name = current->text(0);
        for(int i=0;i<ui->pageWidget->count();i++) {
            auto w = ui->pageWidget->widget(i);
            if(name == w->objectName()) {
                // found the correct page, set to front
                ui->pageWidget->setCurrentWidget(w);
                break;
            }
        }
    });

    // Reset and OK action
    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, [=](){
        if(QMessageBox::question(this, "Restore defaults?", "Do you really want to set all preferences to their default values?") == QMessageBox::StandardButton::Yes) {
            p->setDefault();
            setInitialGUIState();
        }
    });
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, [=](){
        // apply GUI state to settings
        p->Startup.ConnectToFirstDevice = ui->StartupAutoconnect->isChecked();
        p->Startup.RememberSweepSettings = ui->StartupSweepLastUsed->isChecked();
        p->Startup.DefaultSweep.start = ui->StartupSweepStart->value();
        p->Startup.DefaultSweep.stop = ui->StartupSweepStop->value();
        p->Startup.DefaultSweep.bandwidth = ui->StartupSweepBandwidth->value();
        p->Startup.DefaultSweep.points = ui->StartupSweepPoints->value();
        p->Startup.DefaultSweep.excitation = ui->StartupSweepLevel->value();
        p->Acquisition.alwaysExciteBothPorts = ui->AcquisitionAlwaysExciteBoth->isChecked();
        p->Acquisition.suppressPeaks = ui->AcquisitionSuppressPeaks->isChecked();
        accept();
    });

    setInitialGUIState();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::setInitialGUIState()
{
    ui->StartupAutoconnect->setChecked(p->Startup.ConnectToFirstDevice);
    if(p->Startup.RememberSweepSettings) {
        ui->StartupSweepLastUsed->click();
    } else {
        ui->StartupSweepDefault->click();
    }
    ui->StartupSweepStart->setValueQuiet(p->Startup.DefaultSweep.start);
    ui->StartupSweepStop->setValueQuiet(p->Startup.DefaultSweep.stop);
    ui->StartupSweepBandwidth->setValueQuiet(p->Startup.DefaultSweep.bandwidth);
    ui->StartupSweepPoints->setValue(p->Startup.DefaultSweep.points);
    ui->StartupSweepLevel->setValue(p->Startup.DefaultSweep.excitation);

    ui->AcquisitionAlwaysExciteBoth->setChecked(p->Acquisition.alwaysExciteBothPorts);
    ui->AcquisitionSuppressPeaks->setChecked(p->Acquisition.suppressPeaks);
}

void Preferences::load()
{
    QSettings settings;
    // load settings, using default values if not present
    for(auto d : descr) {
        try {
            d.var.setValue(settings.value(d.name, d.def));
        } catch (const exception& e){
            d.var.setValue(d.def);
        }
    }
}

void Preferences::store()
{
    QSettings settings;
    // store settings
    for(auto d : descr) {
        settings.setValue(d.name, d.var.value());
    }
}

void Preferences::edit()
{
    auto dialog = new PreferencesDialog(this);
    dialog->exec();
}

void Preferences::setDefault()
{
    for(auto d : descr) {
        d.var.setValue(d.def);
    }
}
