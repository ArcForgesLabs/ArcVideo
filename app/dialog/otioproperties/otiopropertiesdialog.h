
#ifndef OTIOPROPERTIESDIALOG_H
#define OTIOPROPERTIESDIALOG_H

#include <QDialog>
#include <QTreeWidget>

#include "common/define.h"
#include "node/project.h"
#include "node/project/sequence/sequence.h"
#include "opentimelineio/timeline.h"

namespace arcvideo {

/**
 * @brief Dialog to load setting for OTIO sequences.
 *
 * Takes a list of Sequences and allows the setting of options for each.
 */
class OTIOPropertiesDialog : public QDialog {
    Q_OBJECT

public:
    OTIOPropertiesDialog(const QList<Sequence*>& sequences, Project* active_project, QWidget* parent = nullptr);

private:
    QTreeWidget* table_ = nullptr;

    const QList<Sequence*> sequences_;

private slots:
    /**
     * @brief Brings up the Sequence settings dialog.
     */
    void SetupSequence();
};

}  // namespace arcvideo

#endif  // OTIOPROPERTIESDIALOG_H
