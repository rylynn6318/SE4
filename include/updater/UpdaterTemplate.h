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
            static void call(F f, Tuple &&t) {
                call_impl<
                        F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)
                >::call(f, std::forward<Tuple>(t));
            }
        };

        template<typename F, typename Tuple, int Total, int... N>
        struct call_impl<F, Tuple, true, Total, N...> {
            static void call(F f, Tuple &&t) {
                f(std::get<N>(std::forward<Tuple>(t))...);
            }
        };
    }

    // user invokes this
    template<typename F, typename Tuple>
    void call(F f, Tuple &&t) {
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
    template<typename... ComponentHandlers>
    class UpdaterTemplate : public se4::Updater {
    private:
        std::function<bool(int)> compare_id;
        std::function<void(ComponentHandlers ...)> callback;

        template<typename... Handlers>
        typename std::enable_if<sizeof...(Handlers) == 0>::type addComponentToSignature() {}

        template<typename Handler, typename... Handlers>
        void addComponentToSignature();

        std::tuple<ComponentHandlers...> tuple;

    public:
        ~UpdaterTemplate() override = default;

        UpdaterTemplate(std::function<void(ComponentHandlers ...)> callback,
                        std::function<bool(int)> compare_id);

        void update(int dt) override;

        void update(int dt, std::tuple<ComponentHandlers...> &t);
    };

    // --------------------------------------------- Implementation --------------------------------------------- //

    template<typename... ComponentHandlers>
    UpdaterTemplate<ComponentHandlers...>::UpdaterTemplate(std::function<void(ComponentHandlers...)> callback,
                                                           std::function<bool(int)> compare_id)
            : compare_id(std::move(compare_id)),
              callback(std::move(callback)) {
        addComponentToSignature<ComponentHandlers...>();
    }

    template<typename... ComponentHandlers>
    void UpdaterTemplate<ComponentHandlers...>::update(int dt) {
        update(dt, tuple);
    }

    template<typename... ComponentHandlers>
    void UpdaterTemplate<ComponentHandlers...>::update(int dt, std::tuple<ComponentHandlers...> &t) {
        for (auto &entity : registeredEntities) {
            std::apply([this, &entity](auto &&... args) { ( parentWorld->unpack(entity, args), ...); }, t);

            if (compare_id(entity.id)) {
                call(callback, t);
            }
        }
    }

    template<typename... ComponentHandlers>
    template<typename Handler, typename... Handlers>
    void UpdaterTemplate<ComponentHandlers...>::addComponentToSignature() {
        using ComponentType = typename Handler::ExposedComponentType;
        signature.addComponent<ComponentType>();
        addComponentToSignature<Handlers ...>();
    }
}

#endif //SE4_UPDATERTEMPLATE_H
