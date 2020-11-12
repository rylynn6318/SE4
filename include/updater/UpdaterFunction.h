//
// Created by ssm on 20. 10. 31..
//

#ifndef SE4_UPDATERFUNCTION_H
#define SE4_UPDATERFUNCTION_H

#include <functional>
#include <utility>
#include "Updater.hpp"

namespace se4 {
    // 업데이터를 새로 상속해서 만들 필요 없이 쓸 수 있는 업데이터 템플릿
    // callback : 각각의 컴포넌트에 대해 실제로 할 동작 함수
    // 템플릿 파라메터 : 사용할 컴포넌트의 핸들러
    template<typename... ComponentHandlers>
    class UpdaterFunction : public se4::Updater {
    private:
        std::function<void(int, ComponentHandlers ...)> callback;
        std::function<bool(void)> init_callback;

        // ComponentHandlers 에서 Component 추출해 signature 추가하는 재귀 템플릿
        template<typename... Handlers>
        auto addComponentToSignature() -> typename std::enable_if<sizeof...(Handlers) == 0>::type {}

        template<typename Handler, typename... Handlers>
        auto addComponentToSignature() -> void {
            // 핸들러의 컴포넌트 추출 해서 시그니처에 추가하는 것을 재귀
            using ComponentType = typename Handler::ExposedComponentType;
            signature.addComponent<ComponentType>();
            addComponentToSignature<Handlers ...>();
        }

    public:
        ~UpdaterFunction() override = default;

        explicit UpdaterFunction(std::function<void(int, ComponentHandlers ...)> callback)
                : callback(std::move(callback)) {
            init_callback = []() -> bool { return true; };
            addComponentToSignature<ComponentHandlers...>();
        }

        UpdaterFunction(std::function<void(int, ComponentHandlers ...)> callback, std::function<bool(void)> init)
                : callback(std::move(callback)),
                  init_callback(std::move(init)) {
            addComponentToSignature<ComponentHandlers...>();
        }

        auto init() -> bool override {
            return init_callback();
        }

        auto update(int dt) -> void override {
            for (auto &entity : registeredEntities) {
                // 튜플에 있는 컴포넌트들을 가져와서 unpack
                std::tuple<ComponentHandlers...> tuple;
                std::apply([this, &entity](auto &&... args) { ( parentWorld->unpack(entity, args), ...); }, tuple);
                // dt랑 위에 tuple 이랑 합침
                auto tuple_with_dt = std::tuple_cat(std::make_tuple(dt), tuple);
                std::apply(callback, tuple_with_dt);
            }
        }
    };

    template<typename F>
    struct UpdaterFunctionWrapper {
        UpdaterFunction<typename F::argument_type> updater;
        explicit UpdaterFunctionWrapper(F func){
            updater = UpdaterFunction<typename F::argument_type>(func);
        }
    };
}

#endif //SE4_UPDATERFUNCTION_H
