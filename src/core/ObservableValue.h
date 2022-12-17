//
// Created by danil on 17.12.2022.
//

#ifndef MEDGRAPHICS_OBSERVABLE_VALUE_H
#define MEDGRAPHICS_OBSERVABLE_VALUE_H

#include <vector>
#include <functional>

template<typename TType>
class ReadOnlyObservableValue {
public:
    typedef std::function<void(const TType& value)> Observer;

    ReadOnlyObservableValue() : value() {}

    explicit ReadOnlyObservableValue(const TType& value) : value(value) {}

    explicit ReadOnlyObservableValue(TType&& value) : value(value) {}

    const TType& get() {
        return value;
    }

    void addObserver(const Observer& observer) {
        observers.push_back(observer);
    }

protected:
    TType value;
    std::vector<Observer> observers;
};

template<typename TType>
class ObservableValue : public ReadOnlyObservableValue<TType> {
public:
    typedef std::function<void(const TType& value)> Observer;

    ObservableValue() : ReadOnlyObservableValue<TType>() {}

    explicit ObservableValue(const TType& value) : ReadOnlyObservableValue<TType>(value) {}

    explicit ObservableValue(TType&& value) : ReadOnlyObservableValue<TType>(value) {}

    void set(const TType& value) {
        if (this->value == value)
            return;
        this->value = value;
        for (auto& observer: this->observers) {
            observer(value);
        }
    }
};


#endif //MEDGRAPHICS_OBSERVABLE_VALUE_H
