#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <random>

namespace Random{
	inline std::mt19937 generate(){
		std::random_device rd{};
		std::seed_seq ss{
			static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
				rd(), rd(), rd(), rd(), rd(), rd(), rd() };
		return std::mt19937{ ss };
	}

	inline std::mt19937 mt{ generate() };

	inline int get(int min, int max){
		return std::uniform_int_distribution{min, max}(mt);
	}

	template <typename T>
	inline T get_uniform(T min, T max){
		return std::uniform_real_distribution<T>{min, max}(mt);
	}

	template <typename T>
	inline T random_normal(T mu, T std){
		return std::normal_distribution<float>{mu, std}(mt);
	}

	// inline float get_normal_float(){
	// 	return std::normal_distribution<float>{0.0, 1.0}(mt);
	// }

	template <typename T>
	T get(T min, T max){
		return std::uniform_int_distribution<T>{min, max}(mt);
	}

	template <typename R, typename S, typename T>
	R get(S min, T max){
		return get<R>(static_cast<R>(min), static_cast<R>(max));
	}

	template <typename T>
	std::vector<T> random_choice(std::vector<T>& choices, int n){
		std::vector<T> result(n, 0);
		int n_choices { static_cast<int>(choices.size()) };
		int rnd {};

		for (int j {}; j<n; ++j){
			rnd = Random::get(0, n_choices - 1);
			result[j] = choices[rnd];
		}
		return result;
	}

	template <typename T>
	std::vector<T> weighted_random_choice(std::vector<T>& choices, std::vector<float>& weights, int n){
		// https://stackoverflow.com/questions/1761626/weighted-random-numbers
		std::vector<T> result {};
		float sum_of_weights {0.0f};
		int n_choices { static_cast<int>(choices.size()) };
		float rnd {0.0f};

		for (int i {}; i<n_choices; ++i){
			sum_of_weights += weights[i];
		}
		for (int j {}; j<n; ++j){
			rnd = Random::get_uniform<float>(0, sum_of_weights);
			for (int i {}; i<n_choices; ++i){
				if (rnd < weights[i]){
					result.push_back(choices[i]);
					break;
				}
			rnd -= weights[i];
			}
		}
		return result;   
	}
}

#endif
