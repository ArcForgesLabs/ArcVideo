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

#ifndef SLIDERBASE_H
#define SLIDERBASE_H

#include <QStackedWidget>

#include "sliderlabel.h"
#include "sliderladder.h"
#include "widget/focusablelineedit/focusablelineedit.h"

namespace arcvideo {

class SliderBase : public QStackedWidget {
    Q_OBJECT

public:
    SliderBase(QWidget* parent = nullptr);

    void SetAlignment(Qt::Alignment alignment);

    [[nodiscard]] bool IsTristate() const;
    void SetTristate();

    void SetFormat(const QString& s, bool plural = false);
    void ClearFormat();

    [[nodiscard]] bool IsFormatPlural() const;

    void SetDefaultValue(const QVariant& v);

    [[nodiscard]] QString GetFormattedValueToString(const QVariant& v) const;

    void InsertLabelSubstitution(const QVariant& value, const QString& label) {
        label_substitutions_.append({value, label});
        UpdateLabel();
    }

    void SetColor(const QColor& c) { label_->SetColor(c); }

public slots:
    void ShowEditor();

protected slots:
    void UpdateLabel();

protected:
    [[nodiscard]] const QVariant& GetValueInternal() const;

    void SetValueInternal(const QVariant& v);

    [[nodiscard]] QString GetFormat() const;

    [[nodiscard]] QString GetFormattedValueToString() const;

    SliderLabel* label() { return label_; }

    [[nodiscard]] virtual QString ValueToString(const QVariant& v) const = 0;

    virtual QVariant StringToValue(const QString& s, bool* ok) const = 0;

    [[nodiscard]] virtual QVariant AdjustValue(const QVariant& value) const;

    [[nodiscard]] virtual bool CanSetValue() const;

    virtual void ValueSignalEvent(const QVariant& value) = 0;

    void changeEvent(QEvent* e) override;

private:
    bool GetLabelSubstitution(const QVariant& v, QString* out) const;

    SliderLabel* label_ = nullptr;

    FocusableLineEdit* editor_ = nullptr;

    QVariant value_;
    QVariant default_value_;

    bool tristate_;

    QString custom_format_;

    bool format_plural_;

    QVector<QPair<QVariant, QString>> label_substitutions_;

private slots:
    void LineEditConfirmed();

    void LineEditCancelled();

    void ResetValue();
};

}  // namespace arcvideo

#endif  // SLIDERBASE_H
