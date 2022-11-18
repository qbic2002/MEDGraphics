//
// Created by danil on 18.11.2022.
//

#ifndef MEDGRAPHICS_EXP_APP_H
#define MEDGRAPHICS_EXP_APP_H


#include <view/core/Context.h>

class ExpApp : public Context {
    void onCreated(const std::vector<std::wstring>& args) override;
};


#endif //MEDGRAPHICS_EXP_APP_H
