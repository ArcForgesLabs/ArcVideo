#ifndef MULTICAMNODE_H
#define MULTICAMNODE_H

#include "node/node.h"
#include "node/output/track/tracklist.h"

namespace arcvideo {

class Sequence;

class MultiCamNode : public Node {
    Q_OBJECT

public:
    MultiCamNode();

    NODE_DEFAULT_FUNCTIONS(MultiCamNode)

    [[nodiscard]] QString Name() const override;
    [[nodiscard]] QString id() const override;
    [[nodiscard]] QVector<CategoryID> Category() const override;
    [[nodiscard]] QString Description() const override;

    [[nodiscard]] ActiveElements GetActiveElementsAtTime(const QString& input, const TimeRange& r) const override;

    void Value(const NodeValueRow& value, const NodeGlobals& globals, NodeValueTable* table) const override;

    void Retranslate() override;

    static const QString kCurrentInput;
    static const QString kSourcesInput;
    static const QString kSequenceInput;
    static const QString kSequenceTypeInput;

    [[nodiscard]] int GetCurrentSource() const { return GetStandardValue(kCurrentInput).toInt(); }

    [[nodiscard]] int GetSourceCount() const;

    static void GetRowsAndColumns(int sources, int* rows, int* cols);
    void GetRowsAndColumns(int* rows, int* cols) const { return GetRowsAndColumns(GetSourceCount(), rows, cols); }

    void SetSequenceType(Track::Type t) { SetStandardValue(kSequenceTypeInput, t); }

    static void IndexToRowCols(int index, int total_rows, int total_cols, int* row, int* col);

    static int RowsColsToIndex(int row, int col, int total_rows, int total_cols) { return col + row * total_cols; }

    [[nodiscard]] Node* GetConnectedRenderOutput(const QString& input, int element = -1) const override;
    [[nodiscard]] bool IsInputConnectedForRender(const QString& input, int element = -1) const override;

    [[nodiscard]] QVector<QString> IgnoreInputsForRendering() const override;

protected:
    void InputConnectedEvent(const QString& input, int element, Node* output) override;
    void InputDisconnectedEvent(const QString& input, int element, Node* output) override;

private:
    [[nodiscard]] TrackList* GetTrackList() const;

    Sequence* sequence_;
};

}  // namespace arcvideo

#endif  // MULTICAMNODE_H
