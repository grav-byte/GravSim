#pragma once
#include "IUserInterface.h"
#include "App/Engine/Prediction/Predictor.h"


class PredictionUI: public IUserInterface {
    public:
        PredictionUI();

        void OnEvent(Core::Event &event) override;
        void Draw() override;

        Predictor::PredictionSettings settings;

    private:
        void QueueDrawPrediction(SceneObject *obj) const;
        void DrawCombo(int &selectedIndex, const std::vector<SceneObject *> &objects);

        glm::vec4 color_{};
        Scene* scene_;
};
