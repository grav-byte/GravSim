from astropy.time import Time
from astropy.coordinates import (
    get_body_barycentric_posvel,
    SkyCoord,
    BarycentricTrueEcliptic,
    CartesianRepresentation,
    CartesianDifferential,
)
import astropy.units as u

# ------------------------------------------------------------
# Normalization
# ------------------------------------------------------------
G_SI = 6.67430e-11
AU_scale = 1.496e11           # m
year = 3.154e7 * 0.2          # s   (0.2 years)
# ------------------------------------------------------------

masses_kg = {
    "sun": 1.9891e30,
    "mercury": 3.3011e23,
    "venus": 4.8675e24,
    "earth": 5.97219e24,
    "mars": 6.4171e23,
    "jupiter": 1.8982e27,
    "saturn": 5.6834e26,
    "uranus": 8.6810e25,
    "neptune": 1.02413e26,
}

def norm_mass(m_kg):
    return G_SI * m_kg * year**2 / AU_scale**3

def norm_velocity(v_m_s):
    return v_m_s * year / AU_scale

t = Time("2026-01-01", scale="tdb")

planets = ["mercury", "venus", "earth", "mars",
           "jupiter", "saturn", "uranus", "neptune"]

# Sun barycentric position and velocity
pos_s, vel_s = get_body_barycentric_posvel("sun", t)

rows = []

for body in planets:
    pos_b, vel_b = get_body_barycentric_posvel(body, t)

    # heliocentric position and velocity
    pos = pos_b - pos_s
    vel = vel_b - vel_s

    # Position + Velocity in Ecliptic frame
    rep = CartesianRepresentation(pos.x, pos.y, pos.z)
    rep = rep.with_differentials(
        CartesianDifferential(vel.x, vel.y, vel.z)
    )
    c = SkyCoord(rep, frame="icrs")
    c_ecl = c.transform_to(BarycentricTrueEcliptic())

    # --- Raw data ---
    r_AU = c_ecl.cartesian.xyz.to(u.AU).value
    v_AU_day = (
        c_ecl.cartesian.differentials["s"]
        .d_xyz.to(u.AU / u.day).value
    )

    # --- Normalizing ---
    v_m_s = v_AU_day * AU_scale / (24 * 3600)
    v_norm = norm_velocity(v_m_s)

    m_norm = norm_mass(masses_kg[body])

    rows.append((body, m_norm, r_AU[0], r_AU[1], v_norm[0], v_norm[1]))

# ------------------------------------------------------------
# Output data
# ------------------------------------------------------------
print("body\tm_norm\tx\ty\tvx\tvy")
for body, m, x, y, vx, vy in rows:
    print(
        f"{body}\t"
        f"{m:.12f}\t"
        f"{x:.6f}\t"
        f"{y:.6f}\t"
        f"{vx:.8f}\t"
        f"{vy:.8f}"
    )