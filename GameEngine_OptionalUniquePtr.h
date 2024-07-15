//Grupp 80
//Adrian Andersson Martvall adan2936
//Ida Laaksonen idla8418
#ifndef OptionalUniquePtr_h
#define OptionalUniquePtr_h

#include <memory>
#include <optional>

/*
 * Vi hade ursprungligen ett externt bibliotek för en optional som kan hantera en unique_ptr, men fick veta
 * det antagligen skulle bli krångligt att inkludera i slutinlämningen, så vi skrev istället
 * en egen version av optionalen. Det är en väldigt enkel optional som endast har en funktion
 * has_value() för att se ifall optionalen har ett värde, samt en funktion get() för att ta ut värdet
 * efter att man kollat om det finns.
 *
 * Att den håller i en nullptr är endast för att Clion annars klagade på att det skulle bli kompileringsfel då
 * instansen skulle hålla i en dangling referens om den skapas utan argument. Det blev det dock inte, men
 * vi tänkte att det var lika bra att ordna det ifall det finns risk för att det annars inte går att
 * kompilera på något visst system.
 */
namespace Engine {
    template<typename T>
    class OptionalUniquePtr {
    public:
        inline bool has_value() {
            return has_val;
        }

        inline const std::unique_ptr<T> &get() {
            if (has_val) return value;
            else throw std::bad_optional_access();
        }

        inline OptionalUniquePtr() : value(null_pointer) {
            has_val = false;
        }

        inline OptionalUniquePtr(std::unique_ptr<T> &ptr) : value(ptr) {
            has_val = true;
        }

        OptionalUniquePtr(const OptionalUniquePtr &other) = delete;

        const OptionalUniquePtr &operator=(const OptionalUniquePtr &other) = delete;

    private:
        //Behövdes för att Clion inte skulle klaga på att en referensens lifetime är för kort
        // när man skapar en unique pointer av en nullptr (Det gav dock inget fel i tester utan endast i IDE:n)
        const std::unique_ptr<T> null_pointer = nullptr;
        //Referens till värdet
        const std::unique_ptr<T> &value;
        bool has_val;
    };
}

#endif
