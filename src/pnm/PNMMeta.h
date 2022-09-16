//
// Created by golov on 16.09.2022.
//

#ifndef MEDGRAPHICS_PNMMETA_H
#define MEDGRAPHICS_PNMMETA_H


#include <vector>
#include <string>

class PNMMeta {
public:
    void addMeta(const std::string& meta, int offset) {
        meta_.emplace_back(meta, offset);
    }

    const std::vector<std::pair<std::string, int>>& getMeta() const {
        return meta_;
    }

private:
    std::vector<std::pair<std::string, int>> meta_;
};


#endif //MEDGRAPHICS_PNMMETA_H
