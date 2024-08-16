#pragma once

template<class Tag>
class NamedBool
{
  enum class Enum { True, False };
public:
  constexpr NamedBool() = default;
  constexpr NamedBool(Enum value) : value(value == Enum::True) {}
  static constexpr NamedBool fromBool(bool value) { return value ? NamedBool::True : NamedBool::False; }
  constexpr bool asBool() const { return this->value; }
  constexpr explicit operator bool() const { return this->asBool(); }
  constexpr bool operator==(const NamedBool& other) const = default;
  static constexpr Enum True = Enum::True;
  static constexpr Enum False = Enum::False;
private:
  bool value = false;
};
