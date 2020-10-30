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

    template<typename... ComponentHandlers>
    class UpdaterTemplate : public se4::Updater {
    private:
        std::function<bool(int)> compare_id;
        std::function<void(ComponentHandlers ...)> callback;

//    template<typename Handler>
//    static void addComponentToSignature(se4::ComponentMask sig) {
//        using ComponentType = typename Handler::ExposedComponentType;
//        sig.addComponent<ComponentType>();
//    }
        template<typename... Handlers>
        typename std::enable_if<sizeof...(Handlers) == 0>::type addComponentToSignature() {
        }

        template<typename Handler, typename... Handlers>
        void addComponentToSignature() {
            using ComponentType = typename Handler::ExposedComponentType;
            signature.addComponent<ComponentType>();
            addComponentToSignature<Handlers ...>();
        }

        std::tuple<ComponentHandlers...> tuple;

    public:
        ~UpdaterTemplate() override = default;

        UpdaterTemplate(std::function<void(ComponentHandlers ...)> callback,
                        std::function<bool(int)> compare_id);

        void update(int dt) override;

        // handle and args : Parameter for callback (and its type is ComponentHandles<ComponentType>)
//    void update(int dt, ComponentHandlers &... args) {
//        for (auto &entity : registeredEntities) {
//            parentWorld->unpack(entity, args...);
//
//            if (compare_id(entity.id)) {
//                callback(args...);
//            }
//        }
//    }
        // handle and args : Parameter for callback (and its type is ComponentHandles<ComponentType>)
        void update(int dt, std::tuple<ComponentHandlers...> &t);
    };

    template<typename... ComponentHandlers>
    UpdaterTemplate<ComponentHandlers...>::UpdaterTemplate(std::function<void(ComponentHandlers...)> callback,
                                                           std::function<bool(int)> compare_id)
            : compare_id(std::move(compare_id)),
              callback(std::move(callback)) {
//        signature.addComponent<Position3f>();
//        signature.addComponent<Volume4f>();
        // signature.addComponent<ComponentTypes ...>();
        // addComponentToSignature<Position3f>(signature);
        addComponentToSignature<ComponentHandlers...>();
    }

    template<typename... ComponentHandlers>
    void UpdaterTemplate<ComponentHandlers...>::update(int dt) {
//        se4::ComponentHandle<Position3f> pos3fHandler;
//        se4::ComponentHandle<Volume4f> volume4fHandler;
        update(dt, tuple);
//        update(dt, pos3fHandler, volume4fHandler);
    }

    template<typename... ComponentHandlers>
    void UpdaterTemplate<ComponentHandlers...>::update(int dt, std::tuple<ComponentHandlers...> &t) {
        for (auto &entity : registeredEntities) {
            std::apply([this, &entity](auto &&... args) { ( parentWorld->unpack(entity, args), ...); }, t);
            // call([this, &entity](auto t) -> auto { parentWorld->unpack(entity, t);}, tuple); // (entity, args...);

            if (compare_id(entity.id)) {
                call(callback, t);
                // callback(args...);
            }
        }
    }

// -------------------- -------------------- -------------------- --------------------
}

#endif //SE4_UPDATERTEMPLATE_H
