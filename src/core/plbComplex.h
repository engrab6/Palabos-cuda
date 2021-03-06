/* This file is part of the Palabos library.
 *
 * Copyright (C) 2011-2013 FlowKit Sarl
 * Route d'Oron 2
 * 1010 Lausanne, Switzerland
 * E-mail contact: contact@flowkit.com
 *
 * The most recent release of Palabos can be downloaded at 
 * <http://www.palabos.org/>
 *
 * The library Palabos is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * The library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLB_COMPLEX_H
#define PLB_COMPLEX_H

namespace plb {

template<typename T>
class Complex {
public:
    Complex();
    Complex(T Re_);
    Complex(T Re_, T Imag_);
    template<typename U> operator U() const;
    T real() const;
    T imaginary() const;
    T modulus() const;
    T sqrModulus() const;
    Complex<T> conjugate() const;
    T argument() const;
    Complex<T> intpow(int n) const;
    Complex<T>& operator+=(Complex<T> const& rhs);
    template<typename U> Complex<T>& operator+=(U rhs);
    Complex<T>& operator-=(Complex<T> const& rhs);
    template<typename U> Complex<T>& operator-=(U rhs);
    Complex<T> operator-() const;
    Complex<T>& operator*=(Complex<T> const& rhs);
    template<typename U> Complex<T>& operator*=(U rhs);
    Complex<T>& operator/=(Complex<T> const& rhs);
    template<typename U> Complex<T>& operator/=(U rhs);
private:
    T Re, Imag;
    static T pi;
};

template<typename T>
Complex<T> operator+(Complex<T> const& arg1, Complex<T> const& arg2);

template<typename T, typename U>
Complex<T> operator+(Complex<T> const& arg1, U arg2);

template<typename T, typename U>
Complex<U> operator+(T arg1, Complex<U> const& arg2);


template<typename T>
Complex<T> operator-(Complex<T> const& arg1, Complex<T> const& arg2);

template<typename T, typename U>
Complex<T> operator-(Complex<T> const& arg1, U arg2);

template<typename T, typename U>
Complex<U> operator-(T arg1, Complex<U> const& arg2);


template<typename T>
Complex<T> operator*(Complex<T> const& arg1, Complex<T> const& arg2);

template<typename T, typename U>
Complex<T> operator*(Complex<T> const& arg1, U arg2);

template<typename T, typename U>
Complex<U> operator*(T arg1, Complex<U> const& arg2);


template<typename T>
Complex<T> operator/(Complex<T> const& arg1, Complex<T> const& arg2);

template<typename T, typename U>
Complex<T> operator/(Complex<T> const& arg1, U arg2);

template<typename T, typename U>
Complex<U> operator/(T arg1, Complex<U> const& arg2);

}  // namespace plb

#endif  // PLB_COMPLEX_H
