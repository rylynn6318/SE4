//
// Created by ssm on 20. 10. 31..
//

#ifndef SE4_UPDATERFUNCTION_H
#define SE4_UPDATERFUNCTION_H

#include <functional>
#include <utility>
#include "Updater.hpp"

namespace se4 {
// -------------------- -------------------- NEW UPDATER -------------------- --------------------
// dark magic source from below link
// https://stackoverflow.com/questions/10766112/c11-i-can-go-from-multiple-args-to-tuple-but-can-i-go-from-tuple-to-multiple
// implementation details, users never invoke these directly
    namespace darkmagic {
        template<typename F, std::integral Int, typename Tuple, bool Done, int Total, int... N>
        struct call_impl {
            static auto call(F f, Int dt, Tuple &&t) -> void {
                call_impl<
                        F, int, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)
                >::call(f, dt, std::forward<Tuple>(t));
            }
        };

        template<typename F, std::integral Int, typename Tuple, int Total, int... N>
        struct call_impl<F, Int, Tuple, true, Total, N...> {
            static auto call(F f, Int dt, Tuple &&t) -> void {
                f(dt, std::get<N>(std::forward<Tuple>(t))...);
            }
        };
    }

    // user invokes this
    template<typename F, std::integral Int, typename Tuple>
    auto call(F f, Int dt, Tuple &&t) -> void {
        typedef typename std::decay<Tuple>::type ttype;
        darkmagic::call_impl<
                F, Int, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value
        >::call(f, dt, std::forward<Tuple>(t));
    }

    // --------------------------------------------- UpdaterFunction --------------------------------------------- //

    // 업데이터를 새로 상속해서 만들 필요 없이 쓸 수 있는 업데이터 템플릿
    // callback : 각각의 컴포넌트에 대해 실제로 할 동작 함수
    // 템플릿 파라메터 : 사용할 컴포넌트의 핸들러
    template<typename... ComponentHandlers>
    class UpdaterFunction : public se4::Updater {
    private:
        std::function<void(int, ComponentHandlers ...)> callback;
        std::function<bool(void)> init_callback;
        // 컴포넌트 값 수정을 위해 존재하는 temp handlers
        // 매 프레임 만드는거 보다(기존 Updater 코드) 멤버변수로 들고 있는게 낫다고 판단함
        std::tuple<ComponentHandlers...> tuple;

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
                std::apply([this, &entity](auto &&... args) { ( parentWorld->unpack(entity, args), ...); }, tuple);
                // 위에서 정의한 흑마법. 튜플들에 있는 컴포넌트들을 파라메터팩으로 넘김
                call(callback, dt, tuple);
            }
        }
    };
}

#endif //SE4_UPDATERFUNCTION_H
