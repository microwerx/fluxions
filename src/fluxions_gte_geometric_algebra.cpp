#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_geometric_algebra.hpp>
#include <fluxions_gte_matrix_stack.hpp>

namespace Fluxions {
	G40vec add_G40_G40(const G40vec& u, const G40vec& v) {
		return G40vec(u.x + v.x);
	}

	G41vec add_G41_G41(const G41vec& u, const G41vec& v) {
		return G41vec(u.e1 + v.e1, u.e2 + v.e2, u.e3 + v.e3, u.e4 + v.e4);
	}

	G42vec add_G42_G42(const G42vec& u, const G42vec& v) {
		return G42vec(u.e1e2 + v.e1e2, u.e1e3 + v.e1e3, u.e1e4 + v.e1e4, u.e2e3 + v.e2e3, u.e2e4 + v.e2e4, u.e3e4 + v.e3e4);
	}

	G43vec add_G43_G43(const G43vec& u, const G43vec& v) {
		return G43vec(u.e1e2e3 + v.e1e2e3, u.e1e2e4 + v.e1e2e4, u.e1e3e4 + v.e1e3e4, u.e2e3e4 + v.e2e3e4);
	}

	G44vec add_G44_G44(const G44vec& u, const G44vec& v) {
		return G44vec(u.e1e2e3e4 + v.e1e2e3e4);
	}
} // namespace Fluxions
