/**
 * Beauty filters and watermarks
 *
 * - For beauty filters, please refer to:updateBeautyStyle:
 * - Set the strength of beauty, skin brightening, and rosy skin effects.
 * - The SDK has two built-in skin smoothing algorithms. One is "smooth", which features more obvious smoothing effect and is designed for showrooms.  The other is "natural", which retains more facial details and is more natural.
 * -
 * - For watermarks, please refer to:  setWatermark()/unsetWatermark()
 * - After you set a watermark, a watermark image will be overlaid on the video seen by remote users. Note that you can add watermarks only to primary-stream videos.
 */

#ifndef TESTBEAUTYANDWATERMARK_H
#define TESTBEAUTYANDWATERMARK_H

#include<QTemporaryDir>
#include<QPixmap>

#include "ITRTCCloud.h"
#include "ui_TestBeautyAndWaterMarkDialog.h"
#include "base_dialog.h"

class TestBeautyAndWaterMark:
        public BaseDialog
{
    Q_OBJECT
public:
    explicit TestBeautyAndWaterMark(QWidget *parent = nullptr);
    ~TestBeautyAndWaterMark();

private:
    void setWatermark();
    void unsetWatermark();
    void updateBeautyStyle();

private slots:
    void on_comboBoxBeautyStyle_currentIndexChanged(int index);

    void on_horizontalSliderBeautyLevel_valueChanged(int value);

    void on_horizontalSliderWhitenessLevel_valueChanged(int value);

    void on_horizontalSliderruddinessLevel_valueChanged(int value);

    void on_checkBoxWaterMark_stateChanged(int check_state);

public:
    void showEvent(QShowEvent *event) override;

private:
    void retranslateUi() override;

private:
    std::unique_ptr<Ui::TestBeautyAndWaterMarkDialog> ui_beauty_and_watermark_;
    liteav::ITRTCCloud *trtccloud;

    QTemporaryDir qtemp_dir_;
    liteav::TRTCBeautyStyle current_beauty_style_ = liteav::TRTCBeautyStyle::TRTCBeautyStyleSmooth;
    uint32_t current_beauty_level_ = 5;
    uint32_t current_whiteness_level_ = 5;
    uint32_t current_ruddiness_level_ = 5;

};

#endif // TESTBEAUTYANDWATERMARK_H
