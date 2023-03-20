#include <atomic>

class TimeManager
{
  public:
    static TimeManager& getInstance()
    {
      // Guaranteed to be destroyed.
      static TimeManager instance; 
      // Instantiated on first use.
      return instance;
    };

    /* Singleton operator to avoid copy */
    TimeManager(TimeManager const&) = delete;
    void operator=(TimeManager const&) = delete;

    /** @brief is the TimeManager initialized? can other use it? */ 
    bool isInitialized() const noexcept { return is_initialized_.load(); }
    /** @brief set if the TimeManager is initialized */ 
    void isInitialized(bool is_initialized) noexcept { is_initialized_.store(is_initialized); }

    /** @brief get the current time in seconds */
    float timeOffsetNs() const noexcept { return time_offset_ns_; }
    /** @brief set the current time in seconds */
    void timeOffsetNs(float time_offset_ns) noexcept { time_offset_ns_ = time_offset_ns; }

    /** @brief get the current low pass filter alpha */
    float lowPassFilterAlpha() const noexcept { return low_pass_filter_alpha_; }
    /** @brief set the current low pass filter alpha */
    void lowPassFilterAlpha(float low_pass_filter_alpha) noexcept { low_pass_filter_alpha_ = low_pass_filter_alpha; }

    /** @brief update the current time offset in seconds using LPF */
    void updateOffsetLPF(float time_offset_ns) noexcept { time_offset_ns_ = time_offset_ns*low_pass_filter_alpha_.load() + (1-low_pass_filter_alpha_.load())*time_offset_ns_; }

  private:
    TimeManager()
    {
      is_initialized_.store(false);
      time_offset_ns_.store(0.0);
      low_pass_filter_alpha_.store(0.0001);
    }

    /** @brief is the TimeManager initialized? can other use it? */
    std::atomic<bool> is_initialized_;
    /** @brief current time offset in seconds */
    std::atomic<float> time_offset_ns_;
    /** @brief current low pass filter alpha */
    std::atomic<float> low_pass_filter_alpha_;


};
