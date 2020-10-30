//
// Created by ssm on 20. 10. 31..
//

#ifndef SE4_UPDATERTEMPLATE_H
#define SE4_UPDATERTEMPLATE_H

#include <functional>
#include "Updater.hpp"

namespace se4 {
// -------------------- -------------------- NEW UPDATER -------------------- --------------------
// dark magic source from below link
// https://stackoverflow.com/questions/10766112/c11-i-can-go-from-multiple-args-to-tuple-but-can-i-go-from-tuple-to-multiple
// implementation details, users never invoke these directly
    namespace darkmagic {
        template<typename F, typename Tuple, bool Done, int Total, int... N>
        struct call_impl {
            static auto call(F f, Tuple &&t) -> void {
                call_impl<
                        F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)
                >::call(f, std::forward<Tuple>(t));
            }
        };

        template<typename F, typename Tuple, int Total, int... N>
        struct call_impl<F, Tuple, true, Total, N...> {
            static auto call(F f, Tuple &&t) -> void {
                f(std::get<N>(std::forward<Tuple>(t))...);
            }
        };
    }

    // user invokes this
    template<typename F, typename Tuple>
    auto call(F f, Tuple &&t) -> void {
        typedef typename std::decay<Tuple>::type ttype;
        darkmagic::call_impl<
                F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value
        >::call(f, std::forward<Tuple>(t));
    }

    // --------------------------------------------- UpdaterTemplate --------------------------------------------- //

    // 업데이터를 새로 상속해서 만들 필요 없이 쓸 수 있는 업데이터 템플릿 클래스
    // callback : 각각의 컴포넌트에 대해 실제로 할 동작 함수
    // compare_id : 특정 id를 가진 엔티티만 callback 을 수행하게 하기 위한 비교문 함수
    // 템플릿 파라메터 : 사용할 컴포넌트의 핸들러
    // TODO : init() 등 기타 함수도 override 할것인가?
    template<typename... ComponentHandlers>
    class UpdaterTemplate : public se4::Updater {
    private:
        std::function<bool(int)> compare_id;
        std::function<void(ComponentHandlers ...)> callback;
        // 컴포넌트 값 수정을 위해 존재하는 temp handlers
        // 매 프레임 만드는거 보다(기존 Updater 코드) 멤버변수로 들고 있는게 낫다고 판단함
        std::tuple<ComponentHandlers...> tuple;

        // ComponentHandlers 에서 Component 추출해 signature 추가하는 재귀 템플릿
        template<typename... Handlers>
        auto addComponentToSignature() -> typename std::enable_if<sizeof...(Handlers) == 0>::type {}

        template<typename Handler, typename... Handlers>
        auto addComponentToSignature() -> void;

    public:
        ~UpdaterTemplate() override = default;

        UpdaterTemplate(std::function<void(ComponentHandlers ...)> callback,
                        std::function<bool(int)> compare_id);

        auto update(int dt) -> void override;

        auto update(int dt, std::tuple<ComponentHandlers...> &t) -> void;
    };

    // --------------------------------------------- Implementation --------------------------------------------- //

    template<typename... ComponentHandlers>
    UpdaterTemplate<ComponentHandlers...>::UpdaterTemplate
            (std::function<void(ComponentHandlers...)> callback, std::function<bool(int)> compare_id)
            : compare_id(std::move(compare_id)),
              callback(std::move(callback)) {
        addComponentToSignature<ComponentHandlers...>();
    }

    template<typename... ComponentHandlers>
    auto UpdaterTemplate<ComponentHandlers...>::update(int dt) -> void {
        update(dt, tuple);
    }

    template<typename... ComponentHandlers>
    auto UpdaterTemplate<ComponentHandlers...>::update(int dt, std::tuple<ComponentHandlers...> &t) -> void {
        for (auto &entity : registeredEntities) {
            // 튜플에 있는 컴포넌트들을 가져와서 unpack
            std::apply([this, &entity](auto &&... args) { ( parentWorld->unpack(entity, args), ...); }, t);

            if (compare_id(entity.id)) {
                // 위에서 정의한 흑마법. 튜플들에 있는 컴포넌트들을 전부 파라메터로 넘김
                call(callback, t);
            }
        }
    }

    template<typename... ComponentHandlers>
    template<typename Handler, typename... Handlers>
    auto UpdaterTemplate<ComponentHandlers...>::addComponentToSignature() -> void {
        // 핸들러의 컴포넌트 추출 해서 시그니처에 추가하는 것을 재귀
        using ComponentType = typename Handler::ExposedComponentType;
        signature.addComponent<ComponentType>();
        addComponentToSignature<Handlers ...>();
    }
}

#endif //SE4_UPDATERTEMPLATE_H
