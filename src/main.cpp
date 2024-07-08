#include <iostream>

template <class T>
class RBGrowingStack {
public:
    RBGrowingStack()  :
        total_size_{ 0 },
        current_array_size_{ 0 },
        arrays_size_{ 0 },
        arrays_ptr_{ Build_() } 
    {
    }
    RBGrowingStack(const T& value) = delete;
    RBGrowingStack(T&& value) = delete;
    ~RBGrowingStack() {
        Destroy_();
    }
    template <typename... Args>
    void Push(const T& value) noexcept {
        if (current_array_size_ == capacity_) {
            Increase_();
        }
        if (std::is_scalar_v<T>) {
            arrays_ptr_[arrays_size_ - 1][current_array_size_] = value;
        }
        else {
            new((arrays_ptr_[arrays_size_ - 1] + current_array_size_)) T(value);
        }
        
        ++current_array_size_;
        ++total_size_;
    }
    void Pop() {
        if (total_size_ > 0) {
            --current_array_size_;
            (arrays_ptr_[arrays_size_ - 1] + (current_array_size_))->~T();
            if (current_array_size_ == 0) {
                ::operator delete[](arrays_ptr_[arrays_size_-1], align_, std::nothrow);
                arrays_ptr_[arrays_size_ - 1] = nullptr;
                --arrays_size_;
                capacity_ /= arrays_size_ == 0 ? 1 : capacity_mult_;
                current_array_size_ = arrays_size_ == 0 ? 0 : capacity_;
            }                
            --total_size_;
        }
    }
    T* Top() {
        return total_size_ > 0 ? (arrays_ptr_[arrays_size_ - 1] + (current_array_size_ - 1)) : nullptr;
    }
    void Clear() {
        Destroy_();
        arrays_ptr_ = Build_();
    }
    bool IsEmpty() const noexcept {
        return total_size_ == 0;
    }
    std::size_t Size() const noexcept{
        return total_size_;
    }
private:
    static constexpr std::align_val_t align_{ alignof(T) };

    static constexpr std::size_t default_arrays_capacity_{ 8 };
    static constexpr std::size_t default_stack_capacity_{ 8 };
    std::size_t capacity_mult_{ 2 };

    std::size_t arrays_capacity_{ default_arrays_capacity_ };
    std::size_t capacity_{ default_stack_capacity_ };

    std::size_t total_size_{ 0 };
    std::size_t current_array_size_{ 0 };
    std::size_t arrays_size_{ 0 };

    T** arrays_ptr_{ nullptr };

    void IncreaseArrays_() {
        arrays_capacity_ *= capacity_mult_;
        T** new_array = new T * [arrays_capacity_];
        for (std::size_t index = 0; index < arrays_size_; ++index) {
            new_array[index] = arrays_ptr_[index];
        }
        delete[] arrays_ptr_;
        arrays_ptr_ = new_array;
    }

    void Increase_() {
        capacity_mult_ *= 2;
        if (arrays_size_ == arrays_capacity_) {
            IncreaseArrays_();
        }
        ++arrays_size_;
        capacity_ *= capacity_mult_;
        T* data_ptr = reinterpret_cast<T*>(new(align_, std::nothrow) char[capacity_ * std::max(sizeof(T), alignof(T))]);
        arrays_ptr_[arrays_size_ - 1] = data_ptr;
        current_array_size_ = 0;
    }

    T** Build_() {
        arrays_ptr_ = new T * [arrays_capacity_];
        T* data_ptr = reinterpret_cast<T*>(new(align_, std::nothrow) char[capacity_ * std::max(sizeof(T), alignof(T))]);
        ++arrays_size_;
        arrays_ptr_[arrays_size_-1] = data_ptr;
        return arrays_ptr_;
    }

    void Destroy_() {
        std::size_t index{ arrays_size_ };
        while (arrays_size_ > 0) {
            --index;
            total_size_ -= current_array_size_;
            std::size_t element{ current_array_size_ };
            if (std::is_scalar_v<T>) {
                current_array_size_ = 0;
            }
            else {
                while (current_array_size_ > 0) {
                    --element;
                    (arrays_ptr_[index] + element)->~T();
                    --current_array_size_;
                }                
            }
            ::operator delete[](arrays_ptr_[index], align_, std::nothrow);
            arrays_ptr_[index] = nullptr;
            --arrays_size_;
          
            capacity_ /= arrays_size_ == 0 ? 1 : capacity_mult_;
            capacity_mult_ /= 2;
            current_array_size_ = arrays_size_ == 0 ? 0 : capacity_;
        }
        delete[] arrays_ptr_;
        arrays_ptr_ = nullptr;
        arrays_capacity_ = default_arrays_capacity_;
    }
};

int main() {
    using namespace std;

    RBGrowingStack<int> stack;
    for (int i = 0; i < 100; i++) {
        cout << i << ", ";
        stack.Push(i);
    }

    RBGrowingStack<string> stack2;
    for (int i = 0; i < 100; i++) {
        cout << i << ", ";
        stack2.Push(to_string(i));
    }

    //cout << endl << "==========================" << endl;
/*     for (int i = 0; i < 100; i++) {
        cout << *(stack.Top()) << ", ";
        stack.Pop();
    } */
    cout << endl;
    return 0;
}