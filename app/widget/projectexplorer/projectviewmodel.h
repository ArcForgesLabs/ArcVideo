/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2022 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <QAbstractItemModel>

#include "node/block/block.h"
#include "node/project.h"
#include "undo/undocommand.h"

namespace arcvideo {

/**
 * @brief An adapter that interprets the data in a Project into a Qt item model for usage in ViewModel Views.
 *
 * Assuming a Project is currently "open" (i.e. the Project is connected to a ProjectExplorer/ProjectPanel through
 * a ProjectViewModel), it may be better to make modifications (e.g. additions/removals/renames) through the
 * ProjectViewModel so that the views can be efficiently and correctly updated. ProjectViewModel contains several
 * "wrapper" functions for Project and Item functions that also signal any connected views to update accordingly.
 */
class ProjectViewModel : public QAbstractItemModel {
    Q_OBJECT

public:
    enum ColumnType {
        /// Media name
        kName,

        /// Media duration
        kDuration,

        /// Media rate (frame rate for video, sample rate for audio)
        kRate,

        /// Last modified time (for footage/files)
        kLastModified,

        /// Creation time (for footage/files)
        kCreatedTime,

        /// Count
        kColumnCount
    };

    static const int kInnerTextRole = Qt::UserRole + 1;

    /**
     * @brief ProjectViewModel Constructor
     *
     * @param parent
     * Parent object for memory handling
     */
    ProjectViewModel(QObject* parent);

    /**
     * @brief Get currently active project
     *
     * @return
     *
     * Currently active project or nullptr if there is none
     */
    [[nodiscard]] Project* project() const;

    /**
     * @brief Set the project to adapt
     *
     * Any views attached to this model will get updated by this function.
     *
     * @param p
     *
     * Project to adapt, can be set to nullptr to "close" the project (will show an empty model that cannot be modified)
     */
    void set_project(Project* p);

    /** Compulsory Qt QAbstractItemModel overrides */
    [[nodiscard]] QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex& child) const override;
    [[nodiscard]] int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    /** Optional Qt QAbstractItemModel overrides */
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                                      int role = Qt::DisplayRole) const override;
    [[nodiscard]] bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    [[nodiscard]] bool canFetchMore(const QModelIndex& parent) const override;

    /** Drag and drop support */
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]] QStringList mimeTypes() const override;
    [[nodiscard]] QMimeData* mimeData(const QModelIndexList& indexes) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                      const QModelIndex& parent) override;

    /**
     * @brief Convenience function for creating QModelIndexes from an Item object
     */
    QModelIndex CreateIndexFromItem(Node* item, int column = 0);

private:
    /**
     * @brief Retrieve the index of `item` in its parent
     *
     * This function will return the index of a specified item in its parent according to whichever sorting algorithm
     * is currently active.
     *
     * @return
     *
     * Index of the specified item, or -1 if the item is root (in which case it has no parent).
     */
    static int IndexOfChild(Node* item);

    /**
     * @brief Retrieves the Item object from a given index
     *
     * A convenience function for retrieving Item objects. If the index is not valid, this returns the root Item.
     */
    [[nodiscard]] Node* GetItemObjectFromIndex(const QModelIndex& index) const;

    /**
     * @brief Check if an Item is a parent of a Child
     *
     * Checks entire "parent hierarchy" of `child` to see if `parent` is one of its parents.
     */
    static bool ItemIsParentOfChild(Folder* parent, Node* child);

    void ConnectItem(Node* n);

    void DisconnectItem(Node* n);

    Project* project_ = nullptr;

private slots:
    void FolderBeginInsertItem(Node* n, int insert_index);

    void FolderEndInsertItem();

    void FolderBeginRemoveItem(Node* n, int child_index);

    void FolderEndRemoveItem();

    void ItemRenamed();
};

}  // namespace arcvideo

#endif  // VIEWMODEL_H
